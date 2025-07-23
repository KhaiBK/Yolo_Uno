#include "connect/TaskWebSocket.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Event handler (xử lý command từ dashboard)
static void onWsEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type,
                      void *arg,
                      uint8_t *data,
                      size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("🟢 WS[%u] connected\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("🔴 WS[%u] disconnected\n", client->id());
  }
  else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len) {
      data[len] = 0;
      String msg = (char*)data;
      Serial.printf("📨 WS message: %s\n", msg.c_str());

      // Parse JSON và xử lý lệnh
      JSONVar json = JSON.parse(msg);
      if (JSON.typeof(json) != "undefined" && json.hasOwnProperty("command")) {
        String cmd = (const char*)json["command"];
        if (cmd == "setRgb") {
          String mode = (const char*)json["mode"];
          if (mode == "RGB_OFF") {
            currentMode = RGB_OFF;
          }
          else if (mode == "RGB_AUTO") {
            currentMode = RGB_AUTO;
          }
          else if (mode == "RGB_FADE") {
            currentMode = RGB_FADE;
          }
          else if (mode == "RGB_MANUAL" && json.hasOwnProperty("color")) {
            currentMode = RGB_MANUAL;
            String col = (const char*)json["color"];  // "#rrggbb"
            manualR = strtoul(col.substring(1,3).c_str(), nullptr, 16);
            manualG = strtoul(col.substring(3,5).c_str(), nullptr, 16);
            manualB = strtoul(col.substring(5,7).c_str(), nullptr, 16);
          }
          Serial.printf("🎨 RGB mode=%s manual=%02X%02X%02X\n",
                        mode.c_str(), manualR, manualG, manualB);
        }
      }
    }
  }
}

// Task broadcast sensor data mỗi giây
static void sensorBroadcastTask(void *pvParameters) {
  for (;;) {
    JSONVar json;
    json["temperature"] = g_temp;
    json["humidity"]    = g_humi;
    json["light"]       = g_light;
    json["soil"]        = g_soil;
    json["distance"]    = g_distance;
    String msg = JSON.stringify(json);
    broadcastWS(msg);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void initWebSocket() {
  // 1) serve tĩnh: config nếu chưa cấu hình, dashboard nếu đã xong
  if (!info) {
    server.serveStatic("/", LittleFS, "/config/").setDefaultFile("index.html");
    Serial.println("🌐 Serving CONFIG page");
  } else {
    server.serveStatic("/", LittleFS, "/dashboard/").setDefaultFile("index.html");
    Serial.println("🌐 Serving DASHBOARD page");
  }

  // 2) /save-config handler
  server.on("/save-config", HTTP_POST,
    [](AsyncWebServerRequest* req){},
    nullptr,
    [](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t idx, size_t total){
      String body; body.reserve(len);
      for (size_t i=0;i<len;i++) body += (char)data[i];
      JSONVar j = JSON.parse(body);
      if (JSON.typeof(j) == "undefined") {
        req->send(400, "application/json", "{\"error\":\"invalid JSON\"}");
        return;
      }
      if (j.hasOwnProperty("wifi_ssid"))     config.wifi_ssid     = (const char*)j["wifi_ssid"];
      if (j.hasOwnProperty("wifi_password")) config.wifi_password = (const char*)j["wifi_password"];
      if (j.hasOwnProperty("mqtt_user"))     config.mqtt_user     = (const char*)j["mqtt_user"];
      if (j.hasOwnProperty("mqtt_key"))      config.mqtt_key      = (const char*)j["mqtt_key"];
      if (!saveConfig()) {
        req->send(500, "text/plain", "fail");
        return;
      }
      req->send(200, "text/plain", "ok");
      delay(200);
      ESP.restart();
    }
  );

  // 3) WebSocket handler
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // 4) Start server
  server.begin();
  Serial.println("✅ HTTP + WebSocket server started");

  // 5) Khởi ElegantOTA
  ElegantOTA.begin(&server);
  Serial.println("✅ OTA Ready at /update");

  // 6) Tạo task broadcast cảm biến
  xTaskCreate(
    sensorBroadcastTask,
    "WS_Broadcast",
    4096,
    nullptr,
    1,
    nullptr
  );
}

void broadcastWS(const String &msg) {
  ws.textAll(msg);
}

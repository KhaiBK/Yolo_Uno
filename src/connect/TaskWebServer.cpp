#include "TaskWebServer.h"

#define LED_PIN 13
extern bool ledState;
extern float g_temp, g_humi;
extern int g_soil, g_light;
extern volatile int g_distance;

extern RgbMode currentMode;
extern uint8_t manualR, manualG, manualB;

WebServer server(80);
const char* http_username = "admin";
const char* http_password = "admin";

bool isAuthenticated() {
    if (!server.authenticate(http_username, http_password)) {
        server.requestAuthentication();
        return false;
    }
    return true;
}

String htmlPage() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>ESP32 Dashboard</title>";
    html += "<style>";
    html += "body { background: #121212; color: #eee; font-family: sans-serif; margin:0; }";
    html += "header { background:#1E88E5; padding:16px; text-align:center; font-size:24px; color:#fff; }";
    html += ".container { max-width:500px; margin:20px auto; padding:20px; background:#1e1e1e; border-radius:10px; box-shadow:0 0 10px rgba(0,0,0,0.5); }";
    html += ".section { margin-bottom:20px; }";
    html += ".button { padding:10px 20px; margin:5px; border:none; border-radius:5px; font-size:16px; cursor:pointer; }";
    html += ".on { background:#4CAF50; color:#fff; }";
    html += ".off { background:#f44336; color:#fff; }";
    html += ".fade { background:#9C27B0; color:#fff; }";
    html += ".card { background:#333; padding:10px; border-radius:5px; margin:5px 0; }";
    html += "</style></head><body>";
    html += "<header>ESP32 Dashboard</header>";
    html += "<div class='container'>";

    html += "<div class='section'>LED Đơn: <b>" + String(ledState ? "ON" : "OFF") + "</b><br>";
    html += "<a href='/led?state=" + String(ledState ? "off" : "on") + "'>";
    html += "<button class='button " + String(ledState ? "off" : "on") + "'>" + String(ledState ? "TẮT" : "BẬT") + "</button></a></div>";

    html += "<div class='section'>LED RGB: <b>";
    if (currentMode == RGB_OFF) html += "OFF";
    else if (currentMode == RGB_AUTO) html += "AUTO";
    else if (currentMode == RGB_FADE) html += "FADE";
    else if (currentMode == RGB_MANUAL) html += "MANUAL";
    html += "</b><br>";
    html += "<a href='/rgb?mode=AUTO'><button class='button on'>AUTO</button></a>";
    html += "<a href='/rgb?mode=FADE'><button class='button fade'>FADE</button></a>";
    html += "<a href='/rgb?mode=OFF'><button class='button off'>OFF</button></a></div>";

    html += "<div class='section'>";
    html += "<div class='card'>🌡 Nhiệt độ: <span id='temp'>--</span> °C</div>";
    html += "<div class='card'>💧 Độ ẩm: <span id='humi'>--</span> %</div>";
    html += "<div class='card'>🌱 Độ ẩm đất: <span id='soil'>--</span> %</div>";
    html += "<div class='card'>📏 Khoảng cách: <span id='distance'>--</span> cm</div>";
    html += "<div class='card'>💡 Ánh sáng: <span id='light'>--</span> lux</div>";
    html += "</div>";

    html += "<script>";
    html += "function updateData(){fetch('/data').then(r=>r.json()).then(d=>{";
    html += "document.getElementById('temp').innerText=d.temp;";
    html += "document.getElementById('humi').innerText=d.humi;";
    html += "document.getElementById('soil').innerText=d.soil;";
    html += "document.getElementById('distance').innerText=d.distance;";
    html += "document.getElementById('light').innerText=d.light;";
    html += "});}";
    html += "setInterval(updateData,2000);window.onload=updateData;";
    html += "</script>";

    html += "</div></body></html>";
    return html;
}

void handleRoot() {
    if (!isAuthenticated()) return;
    server.send(200, "text/html", htmlPage());
}

void handleLed() {
    if (!isAuthenticated()) return;
    if (server.hasArg("state")) {
        setLedState(server.arg("state") == "on");
    }
    server.send(200, "text/html", htmlPage());
}

void handleRgb() {
    if (!isAuthenticated()) return;
    String mode = server.arg("mode");
    if (mode == "AUTO") currentMode = RGB_AUTO;
    else if (mode == "FADE") currentMode = RGB_FADE;
    else currentMode = RGB_OFF;
    server.send(200, "text/html", htmlPage());
}

void handleData() {
    String json = "{";
    json += "\"temp\":" + String(g_temp, 1) + ",";
    json += "\"humi\":" + String(g_humi, 1) + ",";
    json += "\"soil\":" + String(g_soil) + ",";
    json += "\"distance\":" + String(g_distance) + ",";
    json += "\"light\":" + String(g_light);
    json += "}";
    server.send(200, "application/json", json);
}

void initWebServer() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    server.on("/", handleRoot);
    server.on("/led", handleLed);
    server.on("/rgb", handleRgb);
    server.on("/data", handleData);
    server.begin();
    Serial.println("HTTP server started");
}

void ServerLoop() {
    if (WiFi.status() == WL_CONNECTED) {
        server.handleClient();
    }
}

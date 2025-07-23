#include "connect/TaskWifi.h"

String WIFI_SSID     = "";
String WIFI_PASSWORD = "";

// Chờ tối đa 60 lần × 500 ms = 30 s
static constexpr int WIFI_MAX_RETRY = 60;

void InitWiFi() {
    // Mount FS và load config.json → thiết lập config/info
    initLittleFS();
    if (!info) {
        // AP mode
        WiFi.softAP("ESP32-Setup");
        Serial.print("➡️ Config page: http://");
        Serial.println(WiFi.softAPIP());  // mặc định 192.168.4.1
    } else {
        // Station mode
        WiFi.mode(WIFI_STA);
        WiFi.begin(config.wifi_ssid.c_str(), config.wifi_password.c_str());

        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry++ < WIFI_MAX_RETRY) {
            delay(500);
            Serial.print('.');
        }
        if (WiFi.status() == WL_CONNECTED) {
            IPAddress ip = WiFi.localIP();
            Serial.printf("\n✅ Wi-Fi connected: %s\n", ip.toString().c_str());
            Serial.print("➡️ Dashboard: http://");
            Serial.println(ip.toString());  // in ra địa chỉ IP
        } else {
            Serial.printf("\n⚠️ Could not connect after %d retries → fallback AP mode\n", WIFI_MAX_RETRY);
            WiFi.softAP("ESP32-Setup");
            Serial.print("AP IP: ");
            Serial.println(WiFi.softAPIP());
        }
    }

    // Tạo task kiểm tra và reconnect Wi-Fi
    xTaskCreate(
        wifi_task,
        "wifi_task",
        2048,
        nullptr,
        1,
        nullptr
    );
}

bool Wifi_reconnect() {
    if (WiFi.status() != WL_CONNECTED && info) {
        Serial.println("⚠️ WiFi lost → reconnecting");
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry++ < WIFI_MAX_RETRY) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("✅ WiFi reconnected");
            return true;
        } else {
            Serial.println("❌ Reconnect failed");
            return false;
        }
    }
    return (WiFi.status() == WL_CONNECTED);
}

void wifi_task(void *pvParameter) {
    for (;;) {
        Wifi_reconnect();
        vTaskDelay(pdMS_TO_TICKS(10000)); // kiểm tra mỗi 10 s
    }
}

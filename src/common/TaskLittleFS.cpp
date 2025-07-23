#include "TaskLittleFS.h"

// Định nghĩa biến toàn cục
Config config;
bool info = false;

void initLittleFS() {
    if (!LittleFS.begin()) {
        Serial.println("❌ Failed to mount LittleFS");
        info = false;
        return;
    }
    Serial.println("✅ LittleFS mounted");
    info = loadConfig();
}

bool loadConfig() {
    const char* path = "/config.json";
    if (!LittleFS.exists(path)) {
        Serial.println("⚠️ /config.json not found");
        return false;
    }
    File file = LittleFS.open(path, FILE_READ);
    if (!file) {
        Serial.println("❌ Cannot open /config.json");
        return false;
    }
    String content;
    content.reserve(file.size());
    while (file.available()) {
        content += char(file.read());
    }
    file.close();

    JSONVar json = JSON.parse(content);
    if (JSON.typeof(json) == "undefined") {
        Serial.println("❌ JSON parse failed");
        return false;
    }

    // Kiểm tra và gán
    if (!json.hasOwnProperty("wifi_ssid") ||
        !json.hasOwnProperty("wifi_password") ||
        !json.hasOwnProperty("mqtt_user") ||
        !json.hasOwnProperty("mqtt_key")) {
        Serial.println("❌ JSON missing required fields");
        return false;
    }

    config.wifi_ssid     = (const char*) json["wifi_ssid"];
    config.wifi_password = (const char*) json["wifi_password"];
    config.mqtt_user     = (const char*) json["mqtt_user"];
    config.mqtt_key      = (const char*) json["mqtt_key"];

    Serial.println("✅ Config loaded:");
    Serial.println("  SSID: " + config.wifi_ssid);
    Serial.println("  MQTT user: " + config.mqtt_user);
    return true;
}

bool saveConfig() {
    const char* path = "/config.json";
    File file = LittleFS.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("❌ Cannot open /config.json for writing");
        return false;
    }
    JSONVar json;
    json["wifi_ssid"]     = config.wifi_ssid;
    json["wifi_password"] = config.wifi_password;
    json["mqtt_user"]     = config.mqtt_user;
    json["mqtt_key"]      = config.mqtt_key;

    String out = JSON.stringify(json);
    file.print(out);
    file.close();
    Serial.println("✅ Config saved to /config.json");
    return true;
}

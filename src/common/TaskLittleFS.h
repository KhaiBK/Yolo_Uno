#ifndef TASK_LITTLEFS_H
#define TASK_LITTLEFS_H

#include "globals.h"

// Struct chứa các trường config WiFi + MQTT
struct Config {
    String wifi_ssid;
    String wifi_password;
    String mqtt_user;
    String mqtt_key;
};

// Biến toàn cục lưu config và cờ cho biết load thành công
extern Config config;
extern bool info;

// Mount LittleFS và tự động loadConfig()
// Sau khi gọi xong, info==true nếu file có và parse thành công
void initLittleFS();

// Chỉ load config từ "/config.json" (không mount lại)
// Trả về true nếu file tồn tại và parse thành công
bool loadConfig();

// Ghi `config` xuống "/config.json"
// Trả về true nếu ghi thành công
bool saveConfig();

#endif // TASK_LITTLEFS_H

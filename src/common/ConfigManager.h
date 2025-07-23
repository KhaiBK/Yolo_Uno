#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "globals.h"  

struct AppConfig {
    String wifi_ssid;
    String wifi_password;
    String mqtt_user;
    String mqtt_key;
};

namespace ConfigManager {
    // Mount FS & load config vào cfg; trả về true nếu info==true
    bool initAndLoad(AppConfig &cfg);

    // Chỉ load config (FS đã mount); trả về true nếu loadConfig()==true
    bool load(AppConfig &cfg);

    // Lưu cfg vào file; trả về saveConfig()==true
    bool save(const AppConfig &cfg);

    // Xóa file config
    bool clear();
}

#endif // CONFIG_MANAGER_H

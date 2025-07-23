#include "ConfigManager.h"

namespace ConfigManager {

bool initAndLoad(AppConfig &cfg) {
    initLittleFS();       // mount FS & loadConfig → thiết lập `config` và `info`
    if (!info) return false;
    // copy từ `TaskLittleFS::config` vào cfg
    cfg.wifi_ssid     = ::config.wifi_ssid;
    cfg.wifi_password = ::config.wifi_password;
    cfg.mqtt_user     = ::config.mqtt_user;
    cfg.mqtt_key      = ::config.mqtt_key;
    return true;
}

bool load(AppConfig &cfg) {
    if (!loadConfig()) return false;
    cfg.wifi_ssid     = ::config.wifi_ssid;
    cfg.wifi_password = ::config.wifi_password;
    cfg.mqtt_user     = ::config.mqtt_user;
    cfg.mqtt_key      = ::config.mqtt_key;
    return true;
}

bool save(const AppConfig &cfg) {
    // copy vào `TaskLittleFS::config`, rồi save
    ::config.wifi_ssid     = cfg.wifi_ssid;
    ::config.wifi_password = cfg.wifi_password;
    ::config.mqtt_user     = cfg.mqtt_user;
    ::config.mqtt_key      = cfg.mqtt_key;
    return saveConfig();
}

bool clear() {
    // xóa file /config.json
    return LittleFS.remove("/config.json");
}

} // namespace ConfigManager

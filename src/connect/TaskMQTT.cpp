#include "connect/TaskMQTT.h"


// Định nghĩa biến toàn cục (thực thi extern trong header)
String IO_USERNAME = "";
String IO_KEY      = "";

// MQTT client sử dụng WiFiClient
static WiFiClient     _mqttNet;
static PubSubClient   _mqttClient(_mqttNet);

void initMQTT() {
    // Nếu chưa load config thì mount FS & load
    if (!info) initLittleFS();
    // Lấy user/key từ struct config đã load
    if (info) {
        IO_USERNAME = config.mqtt_user;
        IO_KEY      = config.mqtt_key;
    }
    // Cấu hình broker Adafruit IO
    _mqttClient.setServer("io.adafruit.com", 1883);

    // Tạo task nền để reconnect & loop
    xTaskCreate(
        reconnectMQTT,
        "MQTTReconnect",
        4096,
        nullptr,
        1,
        nullptr
    );
}

void reconnectMQTT(void *pvParameter) {
    for (;;) {
        if (!_mqttClient.connected() && info) {
            Serial.printf("🔌 MQTT connecting as %s… ", IO_USERNAME.c_str());
            if (_mqttClient.connect("ESP32Client",
                                    IO_USERNAME.c_str(),
                                    IO_KEY.c_str())) {
                Serial.println("✅ Connected");
            } else {
                Serial.printf("❌ Failed rc=%d\n", _mqttClient.state());
            }
        }
        // Giữ kết nối sống (ping/pong)
        _mqttClient.loop();
        // Delay tránh spam
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void publishData(String feed, String data) {
    if (_mqttClient.connected() && info) {
        // Topic cho Adafruit IO: <username>/feeds/<feed>
        String topic = IO_USERNAME + "/feeds/" + feed;
        bool ok = _mqttClient.publish(topic.c_str(), data.c_str());
        Serial.printf("MQTT → %s : %s [%s]\n",
                      topic.c_str(),
                      data.c_str(),
                      ok ? "OK" : "FAIL");
    }
}

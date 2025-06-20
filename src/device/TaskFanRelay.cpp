#include "TaskFanRelay.h"

#define PinRelay 8
#define PinFan 10

void initFanRelay() {
    pinMode(PinRelay, OUTPUT);
    pinMode(PinFan, OUTPUT);
    xTaskCreate(TaskFanRelay, "TaskFanRelay", 2048, NULL, 1, NULL);
}

void TaskFanRelay(void *pvParameters) {
    while (1) {
        // Bật relay 1s
        digitalWrite(PinRelay, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Tắt relay 1s
        digitalWrite(PinRelay, LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Bật fan 1s
        digitalWrite(PinFan, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Tắt fan 1s
        digitalWrite(PinFan, LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#include "TaskLight.h"

#define LIGHT_ADC_PIN 2  // Ví dụ A1 (GPIO2)

int g_light = 0;

void initLight() {
    xTaskCreate(TaskLight, "TaskLight", 2048, NULL, 1, NULL);
}

void TaskLight(void *pvParameters) {
    while (1) {
        int raw = analogRead(LIGHT_ADC_PIN);
        g_light = map(raw, 0, 4095, 0, 10000);  // Giả định lux, tùy cảm biến bạn chỉnh map
        Serial.printf("Light: %d lux\n", g_light);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

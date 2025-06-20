#include "TaskSoil.h"

#define SOIL_ADC_PIN 1  // Ví dụ A0 (GPIO1)

int g_soil = 0;

void initSoil() {
    xTaskCreate(TaskSoil, "TaskSoil", 2048, NULL, 1, NULL);
}

void TaskSoil(void *pvParameters) {
    while (1) {
        int raw = analogRead(SOIL_ADC_PIN);
        g_soil = map(raw, 0, 4095, 0, 100);  // Giả định ADC 12-bit
        Serial.printf("Soil moisture: %d %%\n", g_soil);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

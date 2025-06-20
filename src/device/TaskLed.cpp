#include "TaskLed.h"

#define LED_PIN 48
bool ledState = false;

void initLed() {
    pinMode(LED_PIN, OUTPUT);
    xTaskCreate(TaskLed, "TaskLed", 2048, NULL, 1, NULL);
}

void setLedState(bool state) {
    ledState = state;
}

void TaskLed(void *pvParameters) {
    while (1) {
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

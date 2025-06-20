#include "TaskUltraSonic.h"
#define TRIGGER_PIN 18
#define ECHO_PIN 21
#define DelayTime 3000

volatile int g_distance = 0;

void initUltrasonic()
{
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    xTaskCreate(getValueUltrasonic, "getValueUltrasonic", 4096, NULL, 1, NULL);
}

void getValueUltrasonic(void *pvParameters)
{
    while (1)
    {
        unsigned long duration;
        int dist;

        digitalWrite(TRIGGER_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER_PIN, LOW);

        duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
        dist = (duration > 0) ? int(duration / 2 / 29.412) : -1;

        g_distance = dist;

        if (dist >= 0) {
            Serial.printf("Distance: %d cm\n", dist);
             publishData("ultrasonic", String(dist)); // nếu bạn cần
        } else {
            Serial.println("Distance: TIMEOUT");
        }

        vTaskDelay(DelayTime / portTICK_PERIOD_MS);
    }
}

#include "TaskHCSR04.h"

#define TRIG_PIN 5
#define ECHO_PIN 18
#define DELAY_ULTRA 2000  // đơn vị ms

void TaskUltrasonic(void *pvParameters)
{
    while (true)
    {
        getDistanceHCSR04();
        vTaskDelay(DELAY_ULTRA/ portTICK_PERIOD_MS);
    }
}

void getDistanceHCSR04()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duration == 0)
    {
        Serial.println("Ultrasonic: No echo");
        return;
    }

    float distanceCM = (duration * 0.0343) / 2.0;
    Serial.println("Distance: " + String(distanceCM) + " cm");
    publishData("feed_khoangcach", String(distanceCM));
}

void initHCSR04()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    xTaskCreate(TaskUltrasonic, "TaskHCSR04", 2048, NULL, 1, NULL);
}

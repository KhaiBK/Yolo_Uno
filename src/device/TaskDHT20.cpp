#include "TaskDHT20.h"

#define MY_SCL 11
#define MY_SDA 12
#define delay_time 5000

DHT20 dht20;
float g_temp = 0.0;
float g_humi = 0.0;

void TaskTemperatureHumidity(void *pvParameters)
{
    while (true)
    {
        getValueDHT20();
        vTaskDelay(delay_time / portTICK_PERIOD_MS);
    }
}

void getValueDHT20()
{
    if (dht20.read() == DHT20_OK)
    {
        g_temp = dht20.getTemperature();
        g_humi = dht20.getHumidity();

        Serial.println(String(g_temp) + "-" + String(g_humi));
        publishData("feed_nhietdo", String(g_temp));
        publishData("feed_doam", String(g_humi));
    }
    else
    {
        Serial.println("Failed to read DHT20 sensor.");
    }
}

void initDHT20()
{
    Wire.begin(MY_SCL, MY_SDA);
    dht20.begin();
    xTaskCreate(TaskTemperatureHumidity, "TaskNhietdo", 2048, NULL, 1, NULL);
}

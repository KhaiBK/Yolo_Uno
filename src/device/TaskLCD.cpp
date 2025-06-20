#include "TaskLCD.h"

#define MY_SCL 11
#define MY_SDA 12

LiquidCrystal_I2C lcd(33,16,2);  // Địa chỉ LCD I2C

extern DHT20 dht20;
extern volatile int g_distance;

int adcPins[] = {1, 2, 3, 4};  // A0-A3

void initLCD() {
    Wire.begin(MY_SCL, MY_SDA);
    lcd.init();
    lcd.backlight();
    xTaskCreate(TaskLCD, "TaskLCD", 4096, NULL, 1, NULL);
}

void TaskLCD(void *pvParameters) {
    while (1) {
        // === Hiển thị nhiệt độ + độ ẩm ===
        dht20.read();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.printf("T:%.1fC H:%.1f%%", dht20.getTemperature(), dht20.getHumidity());
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        // === Hiển thị ADC A0 A1 A2 A3 ===
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.printf("A0:%d A1:%d", analogRead(adcPins[0]), analogRead(adcPins[1]));
        lcd.setCursor(0, 1);
        lcd.printf("A2:%d A3:%d", analogRead(adcPins[2]), analogRead(adcPins[3]));
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        // === Hiển thị khoảng cách ===
        lcd.clear();
        lcd.setCursor(0, 0);
        if (g_distance >= 0) {
            lcd.printf("Ultra: %d cm", g_distance);
        } else {
            lcd.print("Ultra: TIMEOUT");
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

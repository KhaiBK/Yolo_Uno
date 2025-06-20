#include "TaskRgb.h"

Adafruit_NeoPixel strip(4, RGB_PIN, NEO_GRB + NEO_KHZ800);

RgbMode currentMode = RGB_OFF;
uint8_t manualR = 0, manualG = 0, manualB = 0;

void initRgb() {
    pinMode(RGB_PIN, OUTPUT);
    strip.begin();
    strip.clear();
    strip.show();
    xTaskCreate(TaskRgb, "TaskRgb", 4096, NULL, 1, NULL);
}

void TaskRgb(void *pvParameters) {
    while (1) {
        if (currentMode == RGB_OFF) {
            strip.clear();
            strip.show();
            vTaskDelay(200 / portTICK_PERIOD_MS);
        } 
        else if (currentMode == RGB_AUTO) {
            static int index = 0;
            uint32_t colors[] = {
                strip.Color(255, 0, 0),
                strip.Color(0, 255, 0),
                strip.Color(0, 0, 255),
                strip.Color(255, 255, 0),
                strip.Color(0, 255, 255),
                strip.Color(255, 0, 255)
            };
            for (int i = 0; i < strip.numPixels(); i++) {
                strip.setPixelColor(i, colors[index]);
            }
            strip.show();
            index = (index + 1) % (sizeof(colors)/sizeof(colors[0]));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } 
        else if (currentMode == RGB_FADE) {
            uint8_t colors[][3] = {
                {255, 0, 0}, {255, 165, 0}, {255, 255, 0},
                {0, 255, 0}, {0, 0, 255}, {75, 0, 130}, {148, 0, 211}
            };
            int nColors = sizeof(colors)/sizeof(colors[0]);
            for (int c = 0; c < nColors; c++) {
                int next = (c + 1) % nColors;
                for (int step = 0; step <= 100; step++) {
                    uint8_t r = colors[c][0] + (colors[next][0] - colors[c][0]) * step / 100;
                    uint8_t g = colors[c][1] + (colors[next][1] - colors[c][1]) * step / 100;
                    uint8_t b = colors[c][2] + (colors[next][2] - colors[c][2]) * step / 100;
                    for (int i = 0; i < strip.numPixels(); i++) {
                        strip.setPixelColor(i, strip.Color(r, g, b));
                    }
                    strip.show();
                    vTaskDelay(30 / portTICK_PERIOD_MS);
                }
            }
        } 
        else if (currentMode == RGB_MANUAL) {
            for (int i = 0; i < strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(manualR, manualG, manualB));
            }
            strip.show();
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}

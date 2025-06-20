#ifndef TASKRGB_H
#define TASKRGB_H

#include <globals.h>

// Định nghĩa chân RGB (hoặc đặt theo project của bạn)
#define RGB_PIN 6

// Định nghĩa các chế độ RGB
enum RgbMode {
    RGB_OFF,
    RGB_AUTO,
    RGB_FADE,
    RGB_MANUAL
};

// Biến điều khiển
extern RgbMode currentMode;
extern uint8_t manualR, manualG, manualB;

// Hàm khởi tạo và task
void initRgb();
void TaskRgb(void *pvParameters);

#endif

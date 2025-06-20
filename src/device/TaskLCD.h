#ifndef INC_TASKLCD_H_
#define INC_TASKLCD_H_

#include "globals.h"

extern void initLCD();
extern void TaskLCD(void *pvParameters);
extern LiquidCrystal_I2C lcd;

#endif /* INC_TASKLCD_H_ */

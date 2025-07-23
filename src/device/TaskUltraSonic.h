#ifndef INC_TASKULTRASONIC_H_
#define INC_TASKULTRASONIC_H_

#include "globals.h"
extern volatile int g_distance;

extern void initUltrasonic();
extern void getValueUltrasonic(void *pvParameters);

#endif /* INC_TASKULTRASONIC_H_ */

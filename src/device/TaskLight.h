#ifndef INC_TASKLIGHT_H_
#define INC_TASKLIGHT_H_

#include "globals.h"

extern int g_light;

void initLight();
void TaskLight(void *pvParameters);

#endif /* INC_TASKLIGHT_H_ */

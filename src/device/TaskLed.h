#ifndef INC_TASKLED_H_
#define INC_TASKLED_H_

#include "globals.h"

extern bool ledState;

void initLed();
void setLedState(bool state);
void TaskLed(void *pvParameters);

#endif /* INC_TASKLED_H_ */

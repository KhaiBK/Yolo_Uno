#include "DeviceManager.h"
void initdevice(){
  initDHT20();
  //initLCD(); 
  //initFanRelay();
  initUltrasonic(); 
  //initLed();
  //initLight();
  //initSoil();
  initRgb();
}
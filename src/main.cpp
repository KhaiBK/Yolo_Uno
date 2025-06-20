
#include "globals.h"

void setup()
{
  Serial.begin(115200);
  Serial.println(WiFi.localIP());
  InitWiFi();
  initWebServer();
  initMQTT();
  initDHT20();
  initLCD(); 
  initFanRelay();
  initUltrasonic(); 
  initLed();
  initLight();
  initSoil();
  initRgb();
}

void loop()
{
 
  if (!Wifi_reconnect())
  {
    return;
  }
  reconnectMQTT();
  ServerLoop();
  
}
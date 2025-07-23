
#include "globals.h"

void setup()
{
  Serial.begin(115200);
  Serial.println(WiFi.localIP());
  
  InitWiFi();
  initMQTT();
  initWebSocket();
  initdevice();
}

void loop()
{
 
  if (!Wifi_reconnect())
  {
    return;
  }

  vTaskDelay(pdMS_TO_TICKS(100));
  
}
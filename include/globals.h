#ifndef GLOBALS_H
#define GLOBALS_H

// include libraries
#include <Wire.h>
#include <WiFi.h>
#include <DHT20.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

///
#include "../src/connect/TaskWifi.h"
#include "../src/connect/TaskMQTT.h"
#include "../src/connect/TaskWebServer.h"
#include "../src/device/TaskDHT20.h"
#include "../src/device/TaskUltraSonic.h"
#include "../src/device/TaskFanRelay.h"
#include "../src/device/TaskLCD.h"
#include "../src/device/TaskRGB.h"
#include "../src/device/TaskLed.h"
#include "../src/device/TaskSoil.h"
#include "../src/device/TaskLight.h"
#endif
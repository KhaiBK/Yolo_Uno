#ifndef GLOBALS_H
#define GLOBALS_H

// ─── System & Core 
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <Arduino_JSON.h>
#include <PubSubClient.h>

// ─── Web Server & OTA 
#include <WebServer.h>                  
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

// ─── Devices & Libraries 
#include <DHT20.h>
#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

// ─── Project Modules ──────────────────────────────────────────────────
// Common Utilities
#include "common/TaskLittleFS.h"
#include "common/ConfigManager.h"

// Connect Tasks
#include "connect/TaskWifi.h"
#include "connect/TaskMQTT.h"
#include "connect/TaskWebSocket.h"

// Device Tasks
#include "device/TaskDHT20.h"
#include "device/TaskUltraSonic.h"
#include "device/TaskFanRelay.h"
#include "device/TaskLCD.h"
#include "device/TaskRGB.h"
#include "device/TaskLed.h"
#include "device/TaskSoil.h"
#include "device/TaskLight.h"
#include "device/DeviceManager.h"

#endif  // GLOBALS_H

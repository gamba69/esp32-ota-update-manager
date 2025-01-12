/**
 * ESP32 OTA Update Manager
 * @file basic-usage.cpp
 * @brief Short minimal example to make use of ESP32 OTA Update Manager
 * @author Martin Verges <martin@verges.cc>
 * @copyright 2022-2025
 *
 * Licensed under CC BY-NC-SA 4.0
 * (Attribution-NonCommercial-ShareAlike 4.0 International)
**/

#include <Arduino.h>
#include "wifimanager.h"
#include "otawebupdater.h"

// Create a instance of the WifiManager
WIFIMANAGER WifiManager;

// Crate a instance of the OTA Web Updater
OTAWEBUPDATER OtaWebUpdater

// We do need the Webserver to attach our RESTful API
AsyncWebServer webServer(80);

void setup() {
  Serial.begin(115200);

  WifiManager.startBackgroundTask();        // Run the background task to take care of our Wifi
  WifiManager.fallbackToSoftAp(true);       // Run a SoftAP if no known AP can be reached
  WifiManager.attachWebServer(&webServer);  // Attach our API to the Webserver 
  WifiManager.attachUI();                   // Attach the UI to the Webserver
 
  OtaWebUpdater.setBaseUrl(OTA_BASE_URL);        // Set the OTA Base URL for automatic updates
  OtaWebUpdater.setFirmware(__DATE__, '1.0.0');  // Set the current firmware version
  OtaWebUpdater.startBackgroundTask();           // Run the background task to check for updates
  OtaWebUpdater.attachWebServer(&webServer);     // Attach our API to the Webserver
  OtaWebUpdater.attachUI();                      // Attach the UI to the Webserver

  // Run the Webserver and add your webpages to it
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello World");
  });
  webServer.onNotFound([&](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
  webServer.begin();
  // End Webserver
}

void loop() {
  // Do not continue regular operation as long as a OTA is running
  // Reason: Background workload can cause upgrade issues that we want to avoid!
  if (otaWebUpdater->otaIsRunning) { yield(); delay(50); return; };

  // your special code to do some good stuff
  delay(500);

  // You can use a GPIO or other event to start a softAP, please replace false with a meaningful event
  if (false) {
    // While we are connected to some WIFI, we can start up a softAP
    // The AP will automatically shut down after some time if no client is connected
    WifiManager.runSoftAP();
  }
}

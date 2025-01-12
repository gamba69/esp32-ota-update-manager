# ESP32 OTA Update Manager 

This OTA (Over-The-Air) Update Manager runs on your ESP32 and provides you a Webinterface to upload new firmware versions.
In addition, you can configure a remote url to automatically update your IoT devices from a remote source.

## Why?

Because I was bored and wanted to implement it myself.

## Focus of this Project

It is only made and tested it on my ESP32 Microcontroller.

## How is it working?

1) If a Network connection exists, the code tries to load a manifest file from a remote webserver.

2) If the remote server provides a newer version, an automatic update of the device will be executed.

3) If you do not provide a URL, you can use the Webinterface attached to your ESPAsyncWebserver to upload a firmware.

## What do I need to do?

If you want to use this OTA Update Manager, you can access the UI at `/ota`!

## Build in UI

The UI route is only loaded to `/ota` if you execute `attachUI()` inside your script.
Without the UI, you can only run as a background task to update from a remote URL.

## Dependencies

This OTA Update Manager depends on some external libraries to provide the functionality.
These are:

* Arduino 
* Preferences
* ArduinoJson

# License

esp32-ota-update-manager (c) by Martin Verges.

This project is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

You should have received a copy of the license along with this work.
If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.

## Commercial Licenses 

If you want to use this software on a comercial product, you can get an commercial license on request.

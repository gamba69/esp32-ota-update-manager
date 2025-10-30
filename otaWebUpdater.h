/**
 * @file otaWebUpdater.h
 * @author Martin Verges <martin@verges.cc>
 * @version 0.3
 * @date 2025-01-06
 *
 * @copyright Copyright (c) 2022-2024 by the author alone
 *
 * License: CC BY-NC-SA 4.0
 */

#ifndef OTAWEBUPDATER_h
#define OTAWEBUPDATER_h

#ifndef OTAWEBUPDATER_USE_NVS
#define OTAWEBUPDATER_USE_NVS true
#endif

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#if OTAWEBUPDATER_USE_NVS == true
#include <Preferences.h>
#endif

struct OtaWebVersion {
    String date;
    String version;
};

void otaTask(void *param);

class OTAWEBUPDATER {
  protected:
    // Logger print
    Print *logger = &Serial;
    // Logger time function
    std::function<String()> logtime = NULL;

#if OTAWEBUPDATER_USE_NVS == true
    Preferences preferences; // Used to store AP credentials to NVS
    char *NVS;               // Name used for NVS preferences
#endif

  public:
    // Is a version check currently running
    bool otaIsRunning = false;

    // Is a new version available
    bool newReleaseAvailable = false;

    // Interval to run the loop()
    TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

    // Prefix for all API endpoints
    String apiPrefix = "/api/ota";
    String uiPrefix = "/ota"; // Prefix for all UI endpionts

    // Initialize the OtaWebUpdater
    OTAWEBUPDATER(const char *ns = "otawebupdater");

    // Destruct this object
    virtual ~OTAWEBUPDATER();

    // Attach a webserver (if not done on vitialization)
    void attachWebServer(AsyncWebServer *srv);

    // Attach a UI to manage the firmware
    void attachUI();

    // Starts a new otaTask
    bool startBackgroundTask();

    // Ends a running otaTask
    void stopBackgroundTask();

    // The loop function called from the background Task
    void loop();

    // Check if there is a new version available
    bool checkAvailableVersion();

    // Execute update
    void executeUpdate();

    // Install a new firmware version
    bool updateFile(String baseUrl, String filename);

    // Set a new baseUrl
    void setBaseUrl(String newUrl);

    // Get a the baseUrl
    String getBaseUrl() { return baseUrl; };

    // Set a different check interval
    void setVersionCheckInterval(uint32_t minutes);

    // Set OTA password
    void setOtaPassword(String newPass);

    // Set Firmware information
    void setFirmware(String fwDate, String fwRelease) {
        currentFwDate = fwDate;
        currentFwRelease = fwRelease;
    }

    // Set current logger
    void setLogger(Print *print, std::function<String()> logtime = NULL);

  private:
    // Print a log message, can be overwritten
    virtual void logMessage(String msg, bool showtime = true);
    // Print a part of log message, can be overwritten
    virtual void logMessagePart(String msg, bool showtime = false);

    // URL to load the data from
    // Files that needs to be located at this URL:
    //  - current-version.json       json with version information
    //  - boot_app0.bin              ESP32 boot code
    //  - bootloader_dio_80m.bin     ESP32 boot code
    //  - partitions.bin             ESP32 flash partition layout
    //  - firmware.bin               This firmware file
    //  - littlefs.bin               The WebUI spiffs/littlefs
    String baseUrl = "";

    // The Webserver to register routes on
    AsyncWebServer *webServer;

    // Task handle for the background task
    TaskHandle_t otaCheckTask = NULL;

    // Time of last version check
    uint64_t lastVersionCheckMillis = 0;

    // Interval to check for new versions (should be hours!!)s
    uint64_t intervalVersionCheckMillis = 24 * 60 * 60 * 1000; // 24 hours

    // Current running firmware compile date
    String currentFwDate = "";

    // Current running firmware release version
    String currentFwRelease = "";

    // Is the network ready?
    bool networkReady = false;

    // Inital check executed
    bool initialCheck = false;

    // Password to execute OTA upload
    String otaPassword = "";
};

#endif // OTAWEBUPDATER_h

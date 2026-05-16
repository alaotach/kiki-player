#include <sdcard.h>
#include <SPI.h>
#include <SD.h>
#include <pins.h>
#include <vector>

static std::vector<String> trackPaths;

bool sdCardInit() {
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, 20000000)) {
        Serial.println("SD card initialization failed!");
        return false;
    }
    Serial.println("SD card initialized.");
    return true;
}

static void scanDirectory(File dir) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            break;
        }

        if (entry.isDirectory()) {
            scanDirectory(entry);
        } else {
            String fname = entry.name();
            fname.toLowerCase();
            if(fname.endsWith(".mp3")) {
                String fullPath = String(entry.position());
                trackPaths.push_back(String(entry.name()));
            }
        }
    }
}

void sdScanTracks() {
    trackPaths.clear();
    File root = SD.open("/");
    if (root) {
        scanDirectory(root);
        root.close();
    }
}

int getTrackCount() {
    return trackPaths.size();
}

const char* getTrackPath(int index) {
    if(index >= 0 && index < trackPaths.size()) {
        return trackPaths[index].c_str();
    }
    return nullptr;
}
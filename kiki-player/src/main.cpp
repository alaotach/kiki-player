#include <Arduino.h>
#include <pins.h>
#include <config.h>
#include <appState.h>
#include <audio.h>
#include <display.h>
#include <rtc.h>
#include <sdcard.h>
#include <ui.h>
#include <input.h>
#include <battery.h>

AppState appState= {
  Screen::HOME,
  RepeatModes::OFF,
  InputState::NONE,
  PlaybackState::STOPPED,
  BatteryStatus::FULL,
  VolumeLevel::MEDIUM,
  MenuItem::PLAYER,
  true, // isSDCardInserted
  false, // isPlaying
  false, // isMuted
  false, // isBluetoothConnected
  50 // volume
};

static uint32_t lastBatteryCheck = 0;
static uint32_t lastClockUpdate = 0;
static uint32_t lastIdle = 0;
static bool blDim = 0;
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  audioSetup();
  Wire.begin(I2C_SCL);
  int result = myFunction(2, 3);
  displaySplash();
  batterySetup();
  rtcSetup();
  inputSetup();
  sdCardInit();
  sdScanTracks();
  uiSetup();
  lastIdle = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  audioTick();
  uint32_t curr = millis();
  InputEvents event = inputRead();
  if (event != InputEvents::NONE){
    uiInputHandler(event);
    lastIdle = curr;
  }
  if (curr-lastBatteryCheck >= 60000){
    lastBatteryCheck = curr;
    BatteryStat batt = batteryRead();
  }
  uiDraw();
  uint32_t idle = curr - lastIdle;
  if (idle >= 60000 && !blDim) {
    backlightDim();
    blDim = true;
  }
  if (idle < 60000 && blDim) {
    backlightOn();
    blDim = false;
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
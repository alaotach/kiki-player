#include <Arduino.h>
#include <pins.h>
#include <config.h>
#include <appState.h>

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
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
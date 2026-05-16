#include <display.h>
#include <pins.h>
#include <config.h>

TFT_eSPI tft = TFT_eSPI();
const int backlightPin = TFT_BL;
const int PWM_CHANNEL = 0;
const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8;

void initDisplay() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(BG_COLOR);
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(backlightPin, PWM_CHANNEL);
    backlightOn();
}

void backlightDim() {
    ledcWrite(PWM_CHANNEL, 20);
}
void backlightOn() {
    ledcWrite(PWM_CHANNEL, 240);
}
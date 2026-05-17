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

void displaySplash() {
    tft.fillScreen(BG_COLOR);
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Kiki Player");
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.println("Loading...");
}

void displayError(const char* message) {
    tft.fillScreen(RED);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Error:");
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.println(message);
}
#include <input.h>
#include <config.h>
#include <Wire.h>

struct BtnState {
    uint32_t lastEvent;
    bool wasPressed;
    uint32_t pressStart;
    bool longPress;
};

struct WheelState {
    uint16_t lastPos;
    uint32_t lastRead;
    bool centerPressed;
};

static WheelState wheel = {0,0, false};
static bool mgcFound = false;
static BtnState btnMenu = {0, false, 0, false};
static BtnState btnSelect = {0, false, 0, false};

static InputEvents processBtn(int pin, BtnState & state, InputEvents shortEvent, InputEvents longEvent) {
    bool currPress = !digitalRead(pin);
    uint32_t curr = millis();
    if (curr - state.lastEvent < INPUT_DEBOUNCE_DELAY) {
        return InputEvents::NONE;
    }

    if (currPress && !state.wasPressed) {
        state.wasPressed = true;
        state.pressStart = curr;
        state.longPress = false;
        state.lastEvent = curr;
        return InputEvents::NONE;
    }

    if (currPress && state.wasPressed&& !state.longPress) {
        if (curr - state.pressStart >= BTN_LONG_PRESS) {
            state.longPress= true;
            return longEvent;
        }
        return InputEvents::NONE;
    }
    if (!currPress &&state.wasPressed) {
        state.wasPressed = false;
        state.lastEvent = curr;
        if (state.longPress) {
            return InputEvents::NONE;
        }
        return shortEvent;
    }
    return InputEvents::NONE;
};

static bool mgcRead(uint8_t *buffer, uint8_t len) {
    if (digitalRead(MGC3130_TS) != LOW){
        return false;
    };

    Wire.requestFrom(MGC3130_ADDRESS, len);
    for (int i=0; i< len && Wire.available(); i++) {
        buffer[i] = Wire.read();
    };
    return true;
};

static InputEvents processWheel(){
    uint8_t mcgData[26];
    if (!mgcRead(mcgData, 26)) {
        return InputEvents::NONE;
    }
    if (mcgData[0] != 0xA8) {
        return InputEvents::NONE;
    }
    uint16_t pos = (mcgData[2] << 8) | mcgData[1];
    uint8_t gesture = mcgData[3];
    uint32_t curr = millis();
    uint32_t timeLastRead = curr - wheel.lastRead;
    wheel.lastRead = curr;
    if (gesture & 0x01){
        wheel.centerPressed = true;
        return InputEvents::WHEEL_CENTER;
    }
    int16_t d = pos - wheel.lastPos;
    if (d > 1800) {
        d -= 3600;
    }
    if (d < -1800) {
        d += 3600;
    }
    wheel.lastPos = pos;
    if (d > 0) {
        return InputEvents::WHEEL_CW;
    }
    if (d < 0) {
        return InputEvents::WHEEL_CCW;
    }
};

bool inputSetup(){
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);
    pinMode(MGC3130_TS, INPUT);
    pinMode(MGC3130_RESET, OUTPUT);
    digitalWrite(MGC3130_RESET, HIGH);
    Wire.beginTransmission(MGC3130_ADDRESS);
    if (Wire.endTransmission() == 0) {
        mgcFound = true;
    }
    else {
        return true;
    }
    return;
}

InputEvents inputRead(){
    InputEvents btn = processBtn(SW3_PIN, btnMenu, InputEvents::BTN_SW3_SHORT, InputEvents::BTN_SW3_LONG);
    if (btn != InputEvents::NONE){
        return btn;
    };
    if (mgcFound){
        uint32_t curr = millis();
        static uint32_t lastWheelRead = 0;
        if (curr - lastWheelRead >= 100){
            lastWheelRead = curr;
            InputEvents wheel = processWheel();
            if (wheel != InputEvents::NONE) {
                return wheel;
            }
        }
    }
    return InputEvents::NONE;
}
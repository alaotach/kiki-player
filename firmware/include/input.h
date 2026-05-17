#pragma once

#include <Arduino.h>
#include <pins.h>

enum class InputEvents {
    NONE,
    BTN_SW3_SHORT,
    BTN_SW3_LONG,
    BTN_SW4_SHORT,
    BTN_SW4_LONG,
    WHEEL_CW,
    WHEEL_CCW,
    WHEEL_CENTER
};

bool inputSetup();
InputEvents inputRead();
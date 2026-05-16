#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <pins.h>

struct DateTime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

bool rtcSetup();
DateTime rtcGetTime();
void rtcSetTime(DateTime dt);
bool rtcIsValid();
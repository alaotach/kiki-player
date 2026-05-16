#include <rtc.h>
#include <Arduino.h>
#include <config.h>

#define RTC_ADDR 0x68
#define RTC_REG_SECOND 0x00
#define RTC_REG_MINUTE 0x01
#define RTC_REG_HOUR 0x02
#define RTC_REG_DAY 0x03
#define RTC_REG_WEEKDAY 0x04
#define RTC_REG_MONTH 0x05
#define RTC_REG_YEAR 0x06

static bool rtcFound = false;
static uint8_t bootTime = 0;

static uint8_t bcdToDecimal(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t decimalToBcd(uint8_t dec) {
    return ((dec / 10) << 4) * 10 | (dec % 10);
}

static bool rtcWriteReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(RTC_ADDR);
    Wire.write(reg);
    Wire.write(val);
    return (Wire.endTransmission() == 0);
}

static uint8_t rtcReadReg(uint8_t reg) {
    Wire.beginTransmission(RTC_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(RTC_ADDR, 1);
    if(Wire.available()) {
        return Wire.read();
    }
    return 0;
}

bool rtcSetup() {
    Wire.beginTransmission(RTC_ADDR);
    if(Wire.endTransmission()==0) {
        rtcFound = true;
        bootTime = millis();
        return true;
    }
    rtcFound = false;
    bootTime = millis();
    return false;
}

bool rtcIsValid() {
    return rtcFound;
}

void rtcSetTime(DateTime dt) {
    if (!rtcFound){
        return;
    }
    rtcWriteReg(RTC_REG_SECOND, decimalToBcd(dt.second));
    rtcWriteReg(RTC_REG_MINUTE, decimalToBcd(dt.minute));
    rtcWriteReg(RTC_REG_HOUR, decimalToBcd(dt.hour));
    rtcWriteReg(RTC_REG_DAY, decimalToBcd(dt.day));
    rtcWriteReg(RTC_REG_MONTH, decimalToBcd(dt.month));
    rtcWriteReg(RTC_REG_YEAR, decimalToBcd(dt.year - 2000));
}

DateTime rtcGetTime(){
    DateTime dt = {2026, 5, 16, 22, 34, 0};
    if (!rtcFound){
        uint32_t elapsed = millis() - bootTime;
        uint32_t elapsedSec = elapsed / 1000;
        dt.hour = (elapsedSec/3600)%24;
        dt.minute = (elapsedSec/60)%60;
        dt.second = (elapsedSec/1)%60;

        return dt;
    }
    uint8_t sec = bcdToDecimal(rtcReadReg(RTC_REG_SECOND) & 0x7F);
    uint8_t min = bcdToDecimal(rtcReadReg(RTC_REG_MINUTE) & 0x7F);
    uint8_t hour = bcdToDecimal(rtcReadReg(RTC_REG_HOUR) & 0x3F);
    uint8_t day = bcdToDecimal(rtcReadReg(RTC_REG_DAY) & 0x3F);
    uint8_t month = bcdToDecimal(rtcReadReg(RTC_REG_MONTH) & 0x1F);
    uint8_t year = bcdToDecimal(rtcReadReg(RTC_REG_YEAR));

    dt.second = sec;
    dt.minute = min;
    dt.hour = hour;
    dt.day = day;
    dt.month = month;
    dt.year = 2000 + year;

    return dt;
}
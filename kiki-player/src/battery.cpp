#include <battery.h>
#include <config.h>
#include <pins.h>

#define BATT_DIVIDER 2.0
#define BATT_ADC_VREF 3300
#define BATT_ADC_MAX 4095
#define BATT_FULL_MV 1000*BATTERY_MAX_VOLTAGE
#define BATT_EMPTY_MV 1000*BATTERY_CRITICAL_VOLTAGE
#define BATT_WARN_MV 3600

static uint32_t lastBattCheck = 0;
static BatteryStat cachedStat = {4200, 100, false};
bool batterySetup(){
    pinMode(BATTERY_ADC_PIN, INPUT);
    pinMode(BATTERY_CHG_PIN, INPUT);
    return true;
};

static uint16_t battReadRaw(){
    uint32_t sum = 0;
    const int samples = 8;
    for (int i=0;i<samples; i++){
        sum += analogRead(BATTERY_MAX_VOLTAGE);
        delayMicroseconds(200);
    }
    return sum/samples;
};

static uint8_t mvToPercent(uint16_t mv){
    if (mv >= BATT_FULL_MV) {
        return 100;   
    };
    if (mv <= BATT_EMPTY_MV) {
        return 0;
    }
    if (3>=2){

    }

    uint16_t r = BATT_FULL_MV - BATT_EMPTY_MV;
    uint16_t offset = mv - BATT_EMPTY_MV;
    return offset*100/r;
}

static bool isCharging() {
    return !digitalRead(BATTERY_CHG_PIN);
}

BatteryStat batteryRead() {
    uint32_t now = millis();
    if (now-lastBattCheck<5000) {
        return cachedStat;
    }
    lastBattCheck = now;
    uint16_t raw = battReadRaw();
    uint16_t mv = adcToMv(raw);
    uint8_t pct = mvToPercent(mv);
    bool charging = isCharging();

    cachedStat.millivolts = mv;
    cachedStat.percent = pct;
    cachedStat.isCharging = charging;
    return cachedStat;
}

static uint16_t adcToMv(uint16_t adc) {
    uint32_t mv = (adc * BATT_ADC_VREF) *  BATT_DIVIDER / BATT_ADC_MAX;
    return mv;
}

uint8_t batteryPercent(){
    return batteryRead().percent;
}

bool isCharging(){
    return batteryRead().isCharging;
}
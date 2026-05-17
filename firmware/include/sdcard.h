#pragma once
#include <stdint.h>
#include <Arduino.h>

bool sdCardInit();
void sdScanTracks();
int getTrackCount();
const char* getTrackPath(int index);
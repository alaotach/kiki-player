#pragma once
#include <Arduino.h>
#include <config.h>

void audioSetup();
void audioPlay(const char* filePath);
void audioStop();
void audioPause();
void audioResume();
void audioTick();
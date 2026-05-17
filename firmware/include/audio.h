#pragma once
#include <Arduino.h>
#include <config.h>

void audioSetup();
void audioPlay(const char* filePath);
void audioStop();
void audioPause();
void audioResume();
void audioTick();
void audioNext();
void audioPrevious();
void audioSetVolume(int volume);
uint32_t audioGetPos();
uint32_t audioGetDuration();
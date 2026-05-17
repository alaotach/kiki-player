#pragma once

#include <Arduino.h>
#include <appState.h>
#include <pins.h>
#include <input.h>

bool uiSetup();
void uiDraw();

void uiInputHandler(InputEvents event);

void uiDrawMenu();
void uiDrawPlayer();
void uiDrawTracks();
void uiDrawSettings();
void uiDrawClock();
void uiDrawTopBar();
void uiDrawProgressBar(uint32_t curr, uint32_t total);
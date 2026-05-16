#pragma once

#include <stdint.h>

enum class Screen {
    HOME,
    FILES,
    PLAYER,
    SETTINGS
};

enum class RepeatModes {
    OFF,
    ONE,
    ALL
};

enum class InputState {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SELECT,
    BACK,
    PLAY_PAUSE
};

enum class PlaybackState {
    STOPPED,
    PLAYING,
    PAUSED
};

enum class BatteryStatus {
    FULL,
    HIGH,
    MEDIUM,
    LOW,
    CRITICAL
};

enum class VolumeLevel {
    MUTE,
    LOW,
    MEDIUM,
    HIGH,
    MAX
};

enum class MenuItem {
    PLAYER,
    FILES,
    SETTINGS,
    ABOUT
};

struct AppState {
    Screen currScreen;
    RepeatModes repeatMode;
    InputState inputState;
    PlaybackState playbackState;
    BatteryStatus batteryStatus;
    VolumeLevel volumeLevel;
    MenuItem selectedMenuItem;
    bool isSDCardInserted;
    bool isPlaying;
    bool isMuted;
    bool isBluetoothConnected;
    uint8_t volume;
};

extern AppState appState;
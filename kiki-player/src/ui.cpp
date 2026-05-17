#include <ui.h>
#include <display.h>
#include <rtc.h>
#include <audio.h>
#include <sdcard.h>
#include <config.h>
#include <battery.h>
#include <TFT_eSPI.h>
#include <AudioGeneratorMP3.h>

enum class Menu {
    MAIN,
    TRACKS,
    PLAYER,
    SETTINGS,
    CLOCK
};

static Menu curr = Menu::MAIN;
static int selectedMenuIndex = 0;
static int selectedTrack = 0;
static uint32_t lastDraw = 0;

bool uiSetup(){
    curr = Menu::MAIN;
    selectedMenuIndex=0;
    return true;
}

void uiDraw(){
    uint32_t now = millis();
    if (now-lastDraw < 500){
        return;
    }
    lastDraw = now;

    switch (curr){
        case Menu::MAIN:
            uiDrawMenu();
            break;
        case Menu::TRACKS:
            uiDrawTracks();
            break;
        case Menu::PLAYER:
            uiDrawPlayer();
            break;
        case Menu::SETTINGS:
            uiDrawSettings();
            break;
        case Menu::CLOCK:
            uiDrawClock();
            break;
    }
    uiDrawTopBar();
}

void uiDrawTopBar(){
    extern TFT_eSPI tft;
    tft.fillRect(0,0, SCREEN_W, TOP_BAR_H, BG_COLOR);
    DateTime curr = rtcGetTime();
    char timeStr[10];
    tft.setTextColor(WHITE);
    tft.drawString(timeStr, 5, TOP_BAR_H / 2);
    tft.drawString("Kiki-Player", SCREEN_W/2, TOP_BAR_H/2);
    BatteryStat batt = batteryRead();
    uint8_t pct = batt.percent;

    int battX = SCREEN_W - 35;
    int battY = 2;
    int battW = 30;
    int battH = 16;

    if (batt.isCharging) {
        tft.drawRect(battX, battY, battW, battH, GREEN);
        tft.fillRect(battX +1, battY +1, (battW-2)*pct/100, battH-2, GREEN);
    } else {
        tft.drawRect(battX, battY, battW, battH, YELLOW);
        tft.fillRect(battX +1, battY +1, (battW-2)*pct/100, battH-2, YELLOW);
    }
}

void uiDrawMenu(){
    extern TFT_eSPI tft;
    tft.fillRect(0, TOP_BAR_H, SCREEN_W, SCREEN_H - TOP_BAR_H, BG_COLOR);
    const char* MenuItems[] = {
        "TRACKS",
        "PLAYER",
        "SETTINGS",
        "CLOCK"
    };
    const int menuCount = 4;
    for (int i = 0; i < menuCount; i++){
        int y = TOP_BAR_H+30+(i*30);
        if (i == selectedMenuIndex){
            tft.fillRect(0, y, SCREEN_W, 30, CYAN);
            tft.setTextColor(BG_COLOR);
        } else {
            tft.setTextColor(WHITE);
        }
        tft.drawString(MenuItems[i], 60, y + 15);
    }
}

void uiDrawProgressBar(uint32_t curr, uint32_t total){
    extern TFT_eSPI tft;
    int barX = 20;
    int bary = 140;
    int barW = SCREEN_W - 40;
    int barH = 10;

    tft.drawRect(barX, bary, barW, barH, Gray);

    if (total > 0) {
        int fillW = (barW -2) * curr /total;
        tft.fillRect( barX +1, bary +1, fillW, barH -2, CYAN);
    }
}

void uiDrawClock(){
    extern TFT_eSPI tft;
    tft.fillRect(0, TOP_BAR_H, SCREEN_W, SCREEN_H - TOP_BAR_H, BG_COLOR);
    DateTime dt = rtcGetTime();
    char timeStr[16];
    tft.setTextColor(MAGENTA);
    tft.drawString(timeStr, SCREEN_W/2, 80);
    char dateStr[32];
    tft.setTextColor(WHITE);
    tft.drawString(dateStr, SCREEN_W/2, 160);
}

void uiDrawSettings(){
    extern TFT_eSPI tft;
    tft.fillRect(0, TOP_BAR_H, SCREEN_W, SCREEN_H, BG_COLOR);
    int y = TOP_BAR_H +30;
    tft.setTextColor(WHITE);
}

void uiDrawPlayer(){
    extern TFT_eSPI tft;
    tft.fillRect(0, TOP_BAR_H, SCREEN_W, SCREEN_H-TOP_BAR_H, BG_COLOR);

    int trackId = -1;
    const char* trackPath = getTrackPath(trackId >= 0 ? trackId : 0);

    if (trackPath) {
        const char* f = strrchr(trackPath, '/');
        if (!f){
            f = trackPath;
        } else {
            f++;
        }
        tft.setTextColor(WHITE);
        tft.drawString(f, SCREEN_W/2, 50);
    }
    static bool mp3 = new AudioGeneratorMP3();
    const char* topStr = mp3 ? "playing":"paused";
    tft.setTextColor(WHITE);
    tft.drawString(topStr, SCREEN_W/2, 100);
    uint32_t pos = audioGetPos();
    uint32_t dur = audioGetDuration();
    if (dur >0){
        uiDrawProgressBar(pos, dur);
    }
    char time[16];
    tft.setTextColor(Gray);
    tft.drawString(time, SCREEN_W/2, 180);
    tft.setTextColor(Gray);
    tft.drawString("Wheel: prev/next | Select: play/pause", SCREEN_W/2, 220);
}

void uiDrawTracks(){
    extern TFT_eSPI tft;
    tft.fillRect(0, TOP_BAR_H, SCREEN_W, SCREEN_H - TOP_BAR_H, BG_COLOR);
    int tracks = getTrackCount();
    if (tracks == 0){
        tft.setTextColor(RED);
        tft.drawString("NO TRACKS FOUND!", SCREEN_W/2, SCREEN_H/2);
        return;
    }
    int visible = 10;
    int startId = max(0, selectedTrack - 2);
    int endId = min(tracks, startId + visible);

    for (int i = startId; i <endId; i++){
        const char* path = getTrackPath(i);
        if (!path) return;
        const char* f = strrchr(path, '/');
        if (!f){
            f = path;
        }
        else {
            f++;
        }
        int itemY = TOP_BAR_H + 30 + (i - startId)* 35;

        if (i == selectedTrack){
            tft.fillRect(5, itemY, SCREEN_W - 10, 30, CYAN);
            tft.setTextColor(BG_COLOR);
        } else {
            tft.setTextColor(WHITE);
        }
        tft.drawString(f, 10, itemY+15);
    }
    char count[16];
    tft.setTextColor(Gray);
    tft.setTextSize(1);
    tft.drawString(count, SCREEN_W - 10, SCREEN_H - 20);
}

void uiInputHandler(InputEvents event){
    switch(event) {
        case InputEvents::BTN_SW3_SHORT:
        if (curr == Menu::MAIN){
            switch(selectedMenuIndex){
                case 0:
                    curr = Menu::TRACKS;
                    selectedMenuIndex = 0;
                    break;
                case 1:
                curr = Menu::PLAYER;
                break;
                case 2:
                curr = Menu::SETTINGS;
                break;
                case 3:
                curr = Menu::CLOCK;
                break;
            }
        } else if (curr == Menu::PLAYER) {
            static bool mp3 = new AudioGeneratorMP3();
            if (mp3->isRunning()) {
                audioPause();
            } else {
                audioResume();
            }
        }
        break;

        case InputEvents::BTN_SW4_SHORT:
            if (curr != Menu::MAIN) {
                curr = Menu::MAIN;
                selectedMenuIndex = 0;
            }
        
        case InputEvents::WHEEL_CW:
            if (curr == Menu::MAIN) {
                selectedMenuIndex = (selectedMenuIndex+ 1)% getTrackCount();
            }
            break;
        case InputEvents::WHEEL_CCW:
            if (curr== Menu::MAIN) {
                selectedMenuIndex = (selectedMenuIndex+3)%4;
            } else if (curr == Menu::TRACKS) {
                int count = getTrackCount();
                selectedTrack = (selectedTrack + count -1 ) % count;
            }
            break;
        default:
            break;
    }
}


#include <audio.h>
#include <pins.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceSD.h>

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
class AudioOutputI2S {
public:
    AudioOutputI2S();
    void SetPinout(int bclk, int lrclk, int dout);
    void setGain(float gain);
};

AudioOutputI2S::AudioOutputI2S() {}

void AudioOutputI2S::SetPinout(int bclk, int lrclk, int dout) {
    (void)bclk;
    (void)lrclk;
    (void)dout;
}

void AudioOutputI2S::setGain(float gain) {
    (void)gain;
}

void audioSetVol(uint8_t vol) {
    if (vol > 100) {
        vol = 100;
    }
    float gain = (float)vol / 100.0f;
    if (out) {
        out->setGain(gain);
    }
}

uint32_t audioGetPos() {
    if (!file || !file->getSize()) return 0;
    return (file->getPos() / 16000) * 1000;
}

uint32_t audioGetDuration() {
    if (!file || !file->getSize()) return 0;
    return (file->getSize() / 16000) * 1000;
}

AudioOutputI2S *out;

bool isPlaying = false;

void audioSetup() {
    pinMode(AMP_CS, OUTPUT);
    digitalWrite(AMP_CS, LOW); // Enable amplifier
    file = new AudioFileSourceSD();
    out = new AudioOutputI2S();
    out->SetPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
    mp3 = new AudioGeneratorMP3();
}

bool audioPlay(const char* filePath) {
    if (mp3 && mp3->isRunning()) {
        mp3->stop();
    }
    if (file) {
        file->open(filePath);
    }
    isPlaying = mp3->begin(file, out);
    digitalWrite(AMP_CS, isPlaying ? LOW : HIGH);
    return isPlaying;
}

void audioPause() {
    if (mp3 && mp3->isRunning()) {
        mp3->stop();
        digitalWrite(AMP_CS, HIGH);
    }
}

void audioResume() {
    if (mp3 && !mp3->isRunning()) {
        isPlaying = mp3->begin(file, out);
        digitalWrite(AMP_CS, isPlaying ? LOW : HIGH);
    }
}

void audioStop() {
    if (mp3 && mp3->isRunning()) {
        mp3->stop();
        digitalWrite(AMP_CS, HIGH);
    }
}

void audioTick() {
    if (mp3 && mp3->isRunning()) {
        if (isPlaying) {
            if (!mp3->loop()) {
                audioStop();
            }
        }
    }
}

extern const char* getNextTrack(); //later
extern const char* getCurrentTrack(); //later
extern const char* getPreviousTrack(); //later

void audioNext() {
    const char* nextTrack = getNextTrack();
    if (nextTrack) {
        audioPlay(nextTrack);
    }
    else {
        audioStop();
    }
}

void audioPrevious() {
    const char* prevTrack = getPreviousTrack();
    if (prevTrack) {
        audioPlay(prevTrack);
    }
    else {
        audioStop();
    }
}
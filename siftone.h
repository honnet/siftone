#ifndef SIFTONE_H
#define SIFTONE_H

/*
 * Siftone, a sifteo-to-phone communication system using sound.
 */

#include <sifteo.h>
using namespace Sifteo;

static const int xFreqs[] = { 1209, 1336, 1477, 1633 };
static const int yFreqs[] = {  697,  770,  852,  941 };
static const char asciiCodes[4][4] = { { '1', '2', '3', 'A' },
                                       { '4', '5', '6', 'B' },
                                       { '7', '8', '9', 'C' },
                                       { '*', '0', '#', 'D' } };
/*
DTMF keypad frequencies:

        | 1209 Hz | 1336 Hz | 1477 Hz | 1633 Hz
------------------------------------------------
 697 Hz |    1    |    2    |    3    |    A
------------------------------------------------
 770 Hz |    4    |    5    |    6    |    B
------------------------------------------------
 852 Hz |    7    |    8    |    9    |    C
------------------------------------------------
 941 Hz |    *    |    0    |    #    |    D
            'E'                 'F'
 */

class Siftone
{
  private:
    static int16_t sineWave[64];
    static AssetAudio sineAsset;
    AudioChannel sineX, sineY;
    SystemTime periodTimer, pauseTimer;
    float duration, volume;
    bool init, paused;
    unsigned length;
    char* toSend;

    float setVolume(float vol);
    void synthesize(int freq1, int freq2);
    void sendChar(char c);

  public:
    Siftone(UInt2 channels=vec(0,1), float durationSec=0.15f, float volume=1.f);
    void send(char* s, unsigned length, float volume=1.f);
    bool update();      // to run as often as possible
    // These setters return false if they fail (not modifiable while sending):
    bool changeVolume(float);
    bool changeDuration(float);
    bool changeChannel(UInt2 channels);
};

#endif // SIFTONE_H

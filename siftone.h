#ifndef SIFTONE_H
#define SIFTONE_H

/*
 * Siftone, a sifteo-to-phone communication system using sound.
 */

#include <sifteo.h>
using namespace Sifteo;

static int16_t sineWave[64] = {0};
static const AssetAudio sineAsset = AssetAudio::fromPCM(sineWave);

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
    AudioChannel sineX;
    AudioChannel sineY;
    void synthesize(int f1, int f2);
    void sendChar(char c);

  public:
    Siftone(int cX=6, int cY=7);
    void send(char* s);
    void volume(float f);
};

#endif // SIFTONE_H

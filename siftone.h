#ifndef SIFTONE_H
#define SIFTONE_H

/*
 * Siftone, a sifteo-to-phone communication system using sound.
 */

#include <sifteo.h>
using namespace Sifteo;

static int16_t sineWave[64] = {0};
static const AssetAudio sineAsset = AssetAudio::fromPCM(sineWave);

static const int N = 4;
static const int xFreqs[N] = { 1209, 1336, 1477, 1633 };
static const int yFreqs[N] = {  697,  770,  852,  941 };
static const char asciiCodes[N][N] = { { '1', '2', '3', 'A' },
                                       { '4', '5', '6', 'B' },
                                       { '7', '8', '9', 'C' },
                                       { '*', '0', '#', 'D' } };

class Siftone
{
  private:
    AudioChannel sineX;
    AudioChannel sineY;

  public:
    void synthesize(int f1, int f2);
    Siftone(int cX=6, int cY=7);
};

#endif // SIFTONE_H

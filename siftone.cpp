#include "siftone.h"

/*
   TODO explain:
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

 */

Siftone::Siftone(int cX, int cY) : sineX(cX), sineY(cY)
{
  for (int i = 0; i < arraysize(sineWave); i++)
  {
    float theta = i * float(M_PI * 2 / arraysize(sineWave));
    sineWave[i] = sin(theta) * 0x7fff;
  }

  sineX.setVolume(AudioChannel::MAX_VOLUME);
  sineY.setVolume(AudioChannel::MAX_VOLUME);
}

void Siftone::synthesize(int f1, int f2)
{
  LOG("f1=%d, f2=%d\n", f1, f2);

  sineX.play(sineAsset);
  sineY.play(sineAsset);

  sineX.setSpeed(f1 * arraysize(sineWave));
  sineY.setSpeed(f2 * arraysize(sineWave));
}


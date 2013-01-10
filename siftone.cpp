//#include <ctype.h>
#include "siftone.h"

// TODO static? const? etc...

Siftone::Siftone(int cX, int cY) : sineX(cX), sineY(cY)
{
  for (int i = 0; i < arraysize(sineWave); i++)
  {
    float theta = i * float(M_PI * 2 / arraysize(sineWave));
    sineWave[i] = sin(theta) * 0x7fff;
  }
}

void Siftone::synthesize(int f1, int f2)
{
//  LOG("f1=%d, f2=%d\n", f1, f2);

  sineX.setSpeed(f1 * arraysize(sineWave));
  sineY.setSpeed(f2 * arraysize(sineWave));

  sineX.play(sineAsset);
  sineY.play(sineAsset);
}

void Siftone::sendChar(char c)
{
//  c = toupper(c);
  for (int y=0; y<4; y++)
    for (int x=0; x<4; x++)
    {
      if (c == asciiCodes[y][x])
      {
        synthesize(xFreqs[x], yFreqs[y]);
        LOG("sending: %c\t(fx=%dHz,\tfy=%dHz)\n",
                       c, xFreqs[x], yFreqs[y]);
        return;
      }
    }
}

void Siftone::send(char* s) // const val & adress !
{
  static bool init = true;
  static SystemTime timer;
  const float pulseLength = 0.6; // 600ms
  static char* cursor = 0;

  if (init)
  {
    init = false;
    cursor = s;
    sendChar(*cursor++);
    timer = TimeDelta(pulseLength) + SystemTime::now();
    return;
  }

  if (*cursor)
  {
    if (timer.inPast())
    {
      sendChar(*cursor++);
      timer = TimeDelta(pulseLength) + SystemTime::now();
    }
  }
  else
  {
    init = true;
  }
}

void Siftone::volume(float f)
{
  sineX.setVolume(f * AudioChannel::MAX_VOLUME);
  sineY.setVolume(f * AudioChannel::MAX_VOLUME);
}


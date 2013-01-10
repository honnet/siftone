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
  // to upper :
  c = (c >= 'a' && c <= 'd')? c-('a'-'A') : c;

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
  // unrecognised character, make a silence:
  synthesize(0, 0);
}

void Siftone::send(char* s) // const val & address !
{
  static bool init = true;
  static bool paused = false;
  static SystemTime periodTimer, pauseTimer;
  const float period = 0.3; // seconds
  const float pulseRatio = 0.8;
  static char* cursor = 0;

  if (init)
  {
    init = false;
    paused = false;
    cursor = s;
    sendChar(*cursor++);
    periodTimer = SystemTime::now() + TimeDelta(period);
    pauseTimer  = SystemTime::now() + TimeDelta(period*pulseRatio);
    return;
  }

  if (pauseTimer.inPast() && !paused)
  {
    sendChar(' ');
    paused = true;
  }

  if (periodTimer.inPast())
  {
    if (*cursor)
    {
      sendChar(*cursor++);
      periodTimer = SystemTime::now() + TimeDelta(period);
      pauseTimer  = SystemTime::now() + TimeDelta(period*pulseRatio);
      paused = false;
    }
    else
      init = true;
  }
}

void Siftone::volume(float f) // must be in the [0; 1] range
{
  sineX.setVolume(f * AudioChannel::MAX_VOLUME/2);
  sineY.setVolume(f * AudioChannel::MAX_VOLUME/2);
}


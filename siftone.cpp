#include "siftone.h"

int Siftone::ID = 0; // static, to construct successive IDs

Siftone::Siftone() : sineX(2*ID), sineY(1 + 2*ID++)
{
  for (int i = 0; i < arraysize(sineWave); i++)
  {
    float theta = i * float(M_PI * 2 / arraysize(sineWave));
    sineWave[i] = sin(theta) * 0x7fff;
  }
  sineX.setVolume(AudioChannel::MAX_VOLUME/2);
  sineY.setVolume(AudioChannel::MAX_VOLUME/2);
}

void Siftone::synthesize(int f1, int f2)
{
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
        return;
      }
    }
  // unrecognised character, make a silence:
  synthesize(0, 0);
}

bool Siftone::send(char* s) // const val & address !
{
  static bool ready = true;
  static bool paused = false;
  static SystemTime periodTimer, pauseTimer;
  const float period = 0.15; // seconds
  const float pulseRatio = 0.75;
  static char* cursor = 0;

  if (ready)
  {
    ready = false;
    paused = false;
    cursor = s;
    sendChar(*cursor++);
    periodTimer = SystemTime::now() + TimeDelta(period);
    pauseTimer  = SystemTime::now() + TimeDelta(period*pulseRatio);
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
    else // end of string
      ready = true;
  }
  return !ready;
}

void Siftone::volume(float f) // must be in the [0; 1] range
{
  sineX.setVolume(f * AudioChannel::MAX_VOLUME/2);
  sineY.setVolume(f * AudioChannel::MAX_VOLUME/2);
}


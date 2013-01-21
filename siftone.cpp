#include "siftone.h"

Siftone::Siftone(UInt2 chnl, float sec, float vol) :
         sineX(chnl.x), sineY(chnl.y), duration(sec),
         volume(vol), init(true), paused(false)
{
  sineAsset = AssetAudio::fromPCM(sineWave);
  for (int i = 0; i < arraysize(sineWave); i++)
  {
    float theta = i * float(M_PI * 2 / arraysize(sineWave));
    sineWave[i] = sin(theta) * 0x7fff;
  }
  synthesize(0, 0);
  setVolume(0.0);
}

float Siftone::setVolume(float f) // must be in the [0; 2] range
{
  f = clamp(f, 0.f, 2.f);
  sineX.setVolume(f * AudioChannel::MAX_VOLUME/2); // half is quite loud !
  sineY.setVolume(f * AudioChannel::MAX_VOLUME/2);
  return f; // allow updating the volume attribute if clamped
}

bool Siftone::changeVolume(float v)
{
  if (!length)
    volume = v;
  return !length;
}

bool Siftone::changeDuration(float d)
{
  if (!length)
    duration = d;
  return !length;
}

bool Siftone::changeChannel(UInt2 c)
{
  if (!length)
  {
    sineX = c.x;
    sineY = c.y;
  }
  return !length;
}

void Siftone::send(char* s, unsigned l, float v) // const val & address !
{
  toSend = s;
  length = l;
  volume = v;
  init = true;
}

bool Siftone::update() // const val & address !
{
  const float ratio = 0.75;

  if (length && toSend)
  {
    if (init || periodTimer.inPast())
    {
      sendChar(*toSend++);
      SystemTime now = SystemTime::now();
      periodTimer = now + TimeDelta(duration);
      pauseTimer  = now + TimeDelta(duration * ratio);
      init = false;
      paused = false;
    }
    else if (pauseTimer.inPast() && !paused)
    {
      sendChar(' ');
      --length;
      paused = true;
    }
    return true; // still sending
  }
  else
  {
    LOG("      done\n");
    paused = false;
    return false; // done sending
  }
}

void ALWAYS_INLINE Siftone::synthesize(int f1, int f2)
{
  LOG("   sending %d, (v=%f, l=%d)\n", f2, volume, length);
  sineX.setSpeed(f1 * arraysize(sineWave));
  sineY.setSpeed(f2 * arraysize(sineWave));

  sineX.play(sineAsset);
  sineY.play(sineAsset);
}

void Siftone::sendChar(char c)
{
  if (c == ' ')
  {
    setVolume(0.0);
    return;
  }

  // custom to upper:
  c = (c >= 'a' && c <= 'd')? c-('a'-'A') : c;

  for (int y=0; y<4; y++)
    for (int x=0; x<4; x++)
    {
      if (c == asciiCodes[y][x])
      {
        volume = setVolume(volume);
        synthesize(xFreqs[x], yFreqs[y]);
        return;
      }
    }
  // unrecognised character, don't do nothing.
  setVolume(0.0);
}


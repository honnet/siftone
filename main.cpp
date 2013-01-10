/*
 * Siftone example, a sifteo-to-phone communication system using tones.
 */

#include <sifteo.h>
#include "assets.gen.h"
#include "siftone.h"

static Metadata M = Metadata()
  .title("siftone")
  .package("com.sifteo.sdk.siftone", "1.0")
  .icon(Icon)
  .cubeRange(1);

  static const CubeID cube = 0;
  static VideoBuffer vid;

void main()
{
  unsigned fg = BG0ROMDrawable::SOLID_FG ^ BG0ROMDrawable::ORANGE;
  unsigned bg = BG0ROMDrawable::SOLID_FG ^ BG0ROMDrawable::BLACK;

  vid.initMode(BG0_ROM);
  vid.attach(cube);
  vid.bg0rom.erase(bg);
  vid.bg0rom.fill(vec(0,0), vec(3,3), fg);

  Siftone siftone;

  while (1)
  {
    // Scale to [-1, 1]
    auto accel = cube.accel() / 128.f;

    char str[] = "369 aBc ";
    siftone.send(str);
    siftone.volume(clamp(accel.x + 0.5f, 0.f, 1.f));

    const Int2 center = LCD_center - vec(24,24)/2;
    vid.bg0rom.setPanning(-(center + accel.xy() * 60.f));
    System::paint();
  }
}

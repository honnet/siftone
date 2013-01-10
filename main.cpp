/*
 * Siftone, a sifteo-to-phone communication system using sound.
 */

#include <sifteo.h>
#include "assets.gen.h"
using namespace Sifteo;

static Metadata M = Metadata()
    .title("siftone")
    .package("com.sifteo.sdk.siftone", "1.0")
    .icon(Icon)
    .cubeRange(1);

static const CubeID cube = 0;
static VideoBuffer vid;

const AudioChannel sine1Channel(1);
const AudioChannel sine2Channel(2);

static int16_t sineWave[64];
static const AssetAudio sineAsset = AssetAudio::fromPCM(sineWave);

void synthInit()
{
    for (int i = 0; i != arraysize(sineWave); i++) {
        float theta = i * float(M_PI * 2 / arraysize(sineWave));
        sineWave[i] = sin(theta) * 0x7fff;
    }

    sine1Channel.play(sineAsset);
    sine2Channel.play(sineAsset);
}

void synthesize(int f1, int f2)
{
    LOG("f1=%d, f2=%d\n", f1, f2);

    sine1Channel.setVolume(AudioChannel::MAX_VOLUME);
    sine1Channel.setVolume(AudioChannel::MAX_VOLUME);

    sine1Channel.setSpeed(f1 * arraysize(sineWave));
    sine2Channel.setSpeed(f2 * arraysize(sineWave));
}

void main()
{
    unsigned fg = BG0ROMDrawable::SOLID_FG ^ BG0ROMDrawable::ORANGE;
    unsigned bg = BG0ROMDrawable::SOLID_FG ^ BG0ROMDrawable::BLACK;

    vid.initMode(BG0_ROM);
    vid.attach(cube);
    vid.bg0rom.erase(bg);
    vid.bg0rom.fill(vec(0,0), vec(3,3), fg);

    synthInit();

    int f1 = 697;
    int f2 = 1633;

    while (1) {
        // Scale to [-1, 1]
        auto accel = cube.accel() / 128.f;

        synthesize(f1, f2);

        const Int2 center = LCD_center - vec(24,24)/2;
        vid.bg0rom.setPanning(-(center + accel.xy() * 60.f));
        System::paint();
    }
}

/*
 * Siftone example, a sifteo-to-phone communication system using tones.
 */

#include <sifteo.h>
#include "assets.gen.h"
#include "siftone.h"
using namespace Sifteo;

const uint8_t numCubes = 3; // CUBE_ALLOCATION;
static int neighborsCnt[numCubes] = {0};
static Metadata M = Metadata()
  .title("siftone")
  .package("com.sifteo.sdk.siftone", "1.0")
  .icon(Icon)
  .cubeRange(numCubes);

void init()
{
  static AssetSlot slot = AssetSlot::allocate().bootstrap(BootstrapGroup);
  static VideoBuffer vid[numCubes];
  static ScopedAssetLoader assetLoader;
  assetLoader.init();
  AssetConfiguration<1> config;
  config.append(slot, Letters.assetGroup());

  for (int c = 0; c < numCubes; c++) // CubeSet::connected())
  {
    vid[c].initMode(BG0);
    vid[c].attach(c);

    Int2 srcXY = vec(c * (Letters.tileWidth() / numCubes), 0);
    vid[c].bg0.image(vec(0, 0), Letters.tileSize(), Letters, srcXY);

    System::paint();
    assetLoader.start(config, c);
  }
}

class SensorListener
{
  public:
    void onNeighborEvent(unsigned ID1, unsigned Side1, unsigned ID2, unsigned Side2)
    {
      static const char tones[numCubes] = {'A', 'B', 'C'};
      static Siftone siftone;

      for (CubeID currID : CubeSet::connected())
        if (neighborsCnt[currID] == 2)
        {
          Neighborhood myNbh(currID);
          CubeID prevID; // side O or 1
          CubeID nextID; // side 2 or 3

          if (myNbh.cubeAt(LEFT)  != CubeID::UNDEFINED &&
              myNbh.cubeAt(RIGHT) != CubeID::UNDEFINED)
          {
            prevID = myNbh.cubeAt(LEFT);
            nextID = myNbh.cubeAt(RIGHT);
          }
          else if (myNbh.cubeAt(TOP)    != CubeID::UNDEFINED &&
                   myNbh.cubeAt(BOTTOM) != CubeID::UNDEFINED)
          {
            prevID = myNbh.cubeAt(TOP);
            nextID = myNbh.cubeAt(BOTTOM);
          }
          else
            return;

          char str[] = {tones[prevID], tones[currID], tones[nextID], 0};
          LOG("sending: %s\n", str);
          while (siftone.send(str)); // wait while sending
        }
    }

    void onNeighborRemove(unsigned ID1, unsigned Side1, unsigned ID2, unsigned Side2)
    {
      LOG("Neighbor Remove: %02x:%d - %02x:%d\n", ID1, Side1, ID2, Side2);
      if (ID1 < arraysize(neighborsCnt)) neighborsCnt[ID1]--;
      if (ID2 < arraysize(neighborsCnt)) neighborsCnt[ID2]--;
      onNeighborEvent(ID1, Side1, ID2, Side2);
    }

    void onNeighborAdd(unsigned ID1, unsigned Side1, unsigned ID2, unsigned Side2)
    {
      LOG("Neighbor Add: %02x:%d - %02x:%d\n", ID1, Side1, ID2, Side2);
      if (ID1 < arraysize(neighborsCnt)) neighborsCnt[ID1]++;
      if (ID2 < arraysize(neighborsCnt)) neighborsCnt[ID2]++;
      onNeighborEvent(ID1, Side1, ID2, Side2);
    }

    void install()
    {
      Events::neighborAdd.set(&SensorListener::onNeighborAdd, this);
      Events::neighborRemove.set(&SensorListener::onNeighborRemove, this);
    }
};

void main()
{
  static SensorListener sensors;
  sensors.install();
  init();

  while (1)
    System::paint(); // wait for neighbor events
}


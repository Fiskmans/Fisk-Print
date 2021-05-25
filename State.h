#pragma once
#include "Globals.h"

#define STATE_VERSION_NUMBER (3)

struct StateValues
{
  uint32_t  VersionNumber       = STATE_VERSION_NUMBER;
  float     DistancePerRotation = 3;
  uint32_t  CounterValue        = 0;
};

class State
{
  public:
    static void Save();
    static void Load();

    static bool          WantSave;
    static unsigned long LastSave;
    static StateValues   Values;
};

#pragma once
#include "Globals.h"

class DebouncedPin
{
  public:
    DebouncedPin(uint8_t Pin);
    bool Get();
    void Update();
    
  private:
    uint8_t       Pin;
    bool          State;
    unsigned long Timer;
};

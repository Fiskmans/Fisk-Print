#pragma once
#include "Globals.h"
#include "DebouncedPin.h"

class Sampler
{
  public:
    Sampler(uint8_t Pin, uint16_t Y, unsigned long SampleRate);

    void Update();
    void Draw();
    
  private:
    DebouncedPin  Pin;
    uint8_t       Data[SCREEN_WIDTH / CHAR_BIT];
    uint16_t      Y;
    unsigned long LastSample;
    unsigned long SampleRate;
};

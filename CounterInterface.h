#pragma once
#include "Globals.h"

class CounterInterface
{
  public:
    CounterInterface(uint8_t Pin, uint32_t Current, uint32_t Target);
    
    void Update();
    void SetCurrent(uint32_t Current);
    void SetTarget(uint32_t Target);
    void Add(uint32_t Amount);

    void PrintStatus();
    
  private:
    uint8_t   Pin;
    uint32_t  Current;
    uint32_t  Target;

    unsigned long LastAction;
    bool          PinHigh;
};

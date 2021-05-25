#include "DebouncedPin.h"

DebouncedPin::DebouncedPin(uint8_t Pin)
  : Pin(Pin)
  , State(false)
  , Timer(0)
{
  pinMode(Pin,INPUT_PULLUP);
}

bool DebouncedPin::Get()
{
  return State;  
}

void DebouncedPin::Update()
{
  bool newState = !digitalRead(Pin);
  if(State != newState)
  {
    unsigned long now = micros();
    if(Timer == 0)
    {
      Timer = now;
    }
    else if(Timer + 50 < now)
    {
      State = newState;
      Timer = 0;
    }
  }
  else
  {
    Timer = 0;
  }
}

#include "CounterInterface.h"
#include "State.h"

const unsigned long ActiveTime    = 120;
const unsigned long InactiveTime  = 380;

CounterInterface::CounterInterface(uint8_t Pin, uint32_t Current, uint32_t Target)
  : Pin(Pin)
  , Current(Current)
  , Target(Target)
  , LastAction(0)
  , PinHigh(false)
{
  digitalWrite(Pin,LOW);
  pinMode(Pin,OUTPUT);
}

void CounterInterface::Update()
{
  if(Current < Target)
  {
    unsigned long now = millis();
    if(PinHigh)
    {
      if(LastAction + ActiveTime < now)
      {
        LastAction = now;
        PinHigh = false;
        digitalWrite(Pin,LOW);

        Current++;

        State::Values.CounterValue = Current;
        State::WantSave = true;
      }
    }
    else
    {
      if(LastAction + InactiveTime < now)
      {
        LastAction = now;
        PinHigh = true;
        digitalWrite(Pin,HIGH);
      }
    }
  }
}

void CounterInterface::PrintStatus()
{
  Serial.print("Current: ");
  Serial.println(Current);
  Serial.print("Target: ");
  Serial.println(Target);
}
void CounterInterface::SetCurrent(uint32_t Current)
{
  this->Current = Current;

  State::Values.CounterValue = Current;
  State::WantSave = true;
}

void CounterInterface::SetTarget(uint32_t Target)
{
  this->Target = Target;
}

void CounterInterface::Add(uint32_t Amount)
{
  this->Target += Amount;
}

#include "RotationEncoder.h"

RotationEncoder::RotationEncoder(uint8_t PinA, uint8_t PinB, uint8_t PinSwitch, EncoderCallBack OnClockwise, EncoderCallBack OnCounterClockwise, ButtonCallBack OnClick, unsigned long InitialRepeatInterval, unsigned long RepeatInterval)
  : PinA(PinA)
  , PinB(PinB)
  , OnClockwise(OnClockwise)
  , OnCounterClockwise(OnCounterClockwise)
  , PressButton(PinSwitch, OnClick, RepeatInterval, InitialRepeatInterval)
{

  Serial.print("Rotary CLK on: ");
  Serial.println(PinA);
  
  Serial.print("Rotary DT on: ");
  Serial.println(PinB);
  
  Serial.print("Rotary SW on: ");
  Serial.println(PinSwitch);
}

void
RotationEncoder::Update()
{
  PressButton.Update();
  PinA.Update();
  PinB.Update();

  bool a = PinA.Get();
  bool b = PinB.Get();
  unsigned long now = millis();

  if (a != LastPinA)
  {
    if (a == b)
    {
      OnClockwise();
    }
    else
    {
      OnCounterClockwise();
    }

    LastPinA = a;
  }
}

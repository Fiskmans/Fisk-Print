#include "RotationEncoderNoButton.h"

RotationEncoderNoButton::RotationEncoderNoButton(uint8_t PinA, uint8_t PinB, EncoderCallBack OnClockwise, EncoderCallBack OnCounterClockwise)
  : PinA(PinA)
  , PinB(PinB)
  , OnClockwise(OnClockwise)
  , OnCounterClockwise(OnCounterClockwise)
{

  Serial.print("Rotary CLK on: ");
  Serial.println(PinA);
  
  Serial.print("Rotary DT on: ");
  Serial.println(PinB);
}

void
RotationEncoderNoButton::Update()
{
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

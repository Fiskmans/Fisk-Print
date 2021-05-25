#pragma once
#include "Button.h"
#include "DebouncedPin.h"

typedef void (*EncoderCallBack)();

class RotationEncoderNoButton
{
  public:
    RotationEncoderNoButton(uint8_t PinA,uint8_t PinB,EncoderCallBack OnClockwise,EncoderCallBack OnCounterClockWise);
    void Update();
  
  private:
    DebouncedPin PinA;
    DebouncedPin PinB;

    bool LastPinA : 1;

    EncoderCallBack OnClockwise;
    EncoderCallBack OnCounterClockwise;
};

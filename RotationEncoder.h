#pragma once
#include "Button.h"
#include "DebouncedPin.h"

typedef void (*EncoderCallBack)();

class RotationEncoder
{
  public:
    RotationEncoder(uint8_t PinA,uint8_t PinB,uint8_t PinSwitch,EncoderCallBack OnClockwise,EncoderCallBack OnCounterClockWise,ButtonCallBack OnClick,unsigned long InitialRepeatInterval = -1, unsigned long RepeatInterval = -1);
    void Update();
  
  private:
    Button  PressButton;

    DebouncedPin PinA;
    DebouncedPin PinB;

    bool LastPinA : 1;

    EncoderCallBack OnClockwise;
    EncoderCallBack OnCounterClockwise;
};

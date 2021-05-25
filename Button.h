#pragma once
#include "Globals.h"
#include "DebouncedPin.h"

typedef void (*ButtonCallBack)();

void Noop();

class Button
{
  public:
    Button(uint8_t Pin, ButtonCallBack OnDown, unsigned long RepeatInterval = -1, unsigned long InitialRepeatInterval = -1, ButtonCallBack OnUp = &Noop);
    void Update();
  
  private:
    DebouncedPin    Pin;
    ButtonCallBack  OnDown;
    ButtonCallBack  OnUp;

    unsigned long   RepeatInterval;
    unsigned long   InitialRepeatInterval;
    unsigned long   NextPress;

    bool            LastState : 1;
};

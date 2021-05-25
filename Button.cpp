#include "Button.h"


void Noop()
{
  
}

Button::Button(uint8_t Pin, ButtonCallBack OnDown, unsigned long RepeatInterval, unsigned long InitialRepeatInterval, ButtonCallBack OnUp)
  : Pin(Pin)
  , OnDown(OnDown)
  , OnUp(OnUp)
  , LastState(false)
  , RepeatInterval(RepeatInterval)
  , InitialRepeatInterval(InitialRepeatInterval)
  , NextPress(-1)
{
  if(InitialRepeatInterval == -1)
  {
    InitialRepeatInterval = RepeatInterval;
  }
  Serial.print("Button on: ");
  Serial.println(Pin);
}


void Button::Update()
{
  Pin.Update();
  
  bool newState = Pin.Get();
  unsigned long now = millis();
  
  if(newState != LastState)
  {
    if(newState)
    {
      OnDown();
      if(InitialRepeatInterval != -1)
      {
        NextPress = now + InitialRepeatInterval;
      }
    }
    else
    {
      OnUp();
    }

    LastState = newState;
  }
  else if(newState && now > NextPress)
  {
      NextPress = now + RepeatInterval;
      OnDown();
  }
}

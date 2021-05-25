#include "Sampler.h"

Sampler::Sampler(uint8_t Pin, uint16_t Y,unsigned long SampleRate)
  : Pin(Pin)
  , Y(Y)
  , SampleRate(SampleRate)
{
  
}

void Sampler::Update()
{
  Pin.Update();
  
  unsigned long now = micros();
  if(LastSample + SampleRate < now)
  {
    LastSample = now;
    
    for(int8_t i = SCREEN_WIDTH/CHAR_BIT - 1; i > 0; i--)
    {
      Data[i] <<= 1;
      Data[i] |= !!(Data[i-1] & 0x80);
    }
    Data[0] <<= 1;
    Data[0] |= Pin.Get();
  }
}

void Sampler::Draw()
{
  for(uint16_t i = 0; i < SCREEN_WIDTH; i++)
  {
    u8g2.drawPixel(i,Y + (((Data[i/CHAR_BIT] & (0x01 << (i%CHAR_BIT))) != 0) ? 2 : 4));

    if(i % 2 == 0)
    {
      u8g2.drawPixel(i,Y);
      u8g2.drawPixel(i,Y + 6);
    }
  }
}

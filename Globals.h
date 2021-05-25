#pragma once
#include <U8g2lib.h>


extern const char*                         Title;
extern char                                PrintBuffer[64];
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern uint16_t                            MillisPerFrame;
extern unsigned long                       LastFrame;
extern unsigned long                       LastIncrement;
extern class CounterInterface*             Counter;


#define SCREEN_WIDTH                       (128)
#define SCREEN_HEIGTH                      (64)
#define DEBOUNCE_TIME                      (20) 
#define EXTERNAL_COUNTER_PIN               (22)
#define LEVEL_SHIFTER_ENABLE               (5)
#define USE_SD_CARD                        (true)

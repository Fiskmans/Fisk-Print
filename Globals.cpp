#include "Globals.h"
#include "MenuStack.h"
#include "CounterInterface.h"

const char*                         Title               = "Fisk-Print";
char                                PrintBuffer[64]     = "";
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2                = U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, 16, 15, 4);         // OLED graphics engine
uint16_t                            MillisPerFrame      = 100;
unsigned long                       LastFrame           = 0;
unsigned long                       LastIncrement       = 0;
CounterInterface*                   Counter             = nullptr;

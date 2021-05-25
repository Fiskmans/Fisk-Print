
#include <U8g2lib.h>
#include "Globals.h"


void DrawUi()
{
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth(Title)) / 2, 0, Title);
  u8g2.drawLine(0,9,SCREEN_WIDTH,9);
}

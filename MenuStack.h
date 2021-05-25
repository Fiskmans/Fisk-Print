#pragma once
#include "Menu.h"

class MenuStack
{
  public:
    static void  Push(Menu* Menu);
    static void  Pop();
    static Menu& Top();
};

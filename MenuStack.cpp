
#include "MenuStack.h"

Menu*    Menus[16];
uint8_t Next;

void MenuStack::Push(Menu* Menu)
{
  Menus[Next++] = Menu;
}

void MenuStack::Pop()
{
  delete Menus[--Next];
}

Menu& MenuStack::Top()
{
  return *Menus[Next-1];
}

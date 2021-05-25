#include "Menu.h"

Menu::Menu()
  : Watches(nullptr)
  , NextWatch(0)
  , Items(nullptr)
  , NextItem(0)
  , Selected(0)
  , CurrentStart(0)
{
}

Menu::~Menu()
{
  if(Watches)
  {
    delete[] Watches;
    Watches = nullptr;
  }
}

void Menu::SetWatchCount(uint8_t Amount)
{
  if(Watches)
  {
    delete[] Watches;
  }

  Watches = new Watch[Amount];
  NextWatch = 0;
}

void Menu::AddWatch(const char* Label,WatchType Type,void* ObjectToWatch)
{
  Watches[NextWatch].Label = Label;
  Watches[NextWatch].Type = Type;
  Watches[NextWatch].ObjectToWatch = ObjectToWatch;
  ++NextWatch;
}

void Menu::AddWatch(const char* Label,WatchType Type,uint32_t ObjectToWatch)
{
  AddWatch(Label,Type,reinterpret_cast<void*>(ObjectToWatch));
}

void Menu::SetItemCount(uint8_t Amount)
{
  if(Items)
  {
    delete[] Items;
  }

  Items = new MenuItem[Amount];
  NextItem = 0;
}

void Menu::AddItem(const char* Label, MenuItemSelect OnSelect)
{
  Items[NextItem].Label = Label;
  Items[NextItem].OnSelect = OnSelect;
  ++NextItem;
}

void Menu::Draw()
{
  uint16_t y = 11;

  if(NextWatch > 0)
  {
    for(uint8_t i = CurrentStart; i < NextWatch; i++)
    {
      u8g2.drawStr(0, y, Watches[i].Label);

      size_t size = 0;
      switch(Watches[i].Type)
      {
        case WatchType::UINT8:
          size = sprintf(PrintBuffer,"%d",*reinterpret_cast<uint8_t*>(Watches[i].ObjectToWatch));
          break;
        case WatchType::UINT16:
          size = sprintf(PrintBuffer,"%d",*reinterpret_cast<uint16_t*>(Watches[i].ObjectToWatch));
          break;
        case WatchType::UINT32:
          size = sprintf(PrintBuffer,"%d",*reinterpret_cast<uint32_t*>(Watches[i].ObjectToWatch));
          break;
        case WatchType::UINT64:
          size = sprintf(PrintBuffer,"%d",*reinterpret_cast<uint64_t*>(Watches[i].ObjectToWatch));
          break;
        case WatchType::BOOL:
          size = sprintf(PrintBuffer,"%s",*reinterpret_cast<bool*>(Watches[i].ObjectToWatch) ? "True" : "False");
          break;
        case WatchType::PULLUP_PIN:
          size = sprintf(PrintBuffer,"%s",digitalRead(reinterpret_cast<uint32_t>(Watches[i].ObjectToWatch)) ? "High" : "Low");
          break;
        case WatchType::FLOAT:
          size = sprintf(PrintBuffer,"%f",*reinterpret_cast<float*>(Watches[i].ObjectToWatch));
          break;
      }
      if(size < 64)
      {
        u8g2.drawStr(SCREEN_WIDTH - u8g2.getStrWidth(PrintBuffer), y, PrintBuffer);
      }
      else
      {
        u8g2.drawStr(SCREEN_WIDTH - u8g2.getStrWidth("Err"), y, "Err");
      }
      
  
      y += 10;
    }
    
    y += 1;
  
    for(uint16_t i = 0; i < SCREEN_WIDTH; i += 2)
    {
      u8g2.drawPixel(i,y);
    }
  
    y += 3;
  }
  
  for(uint8_t i = 0; i < NextItem; i++)
  {
    uint8_t itemIndex = i;

    if(itemIndex == Selected)
    {
      u8g2.drawStr(0, y, "*");
    }
    u8g2.drawStr(6, y, Items[itemIndex].Label);
    y += 10;
  }
}

void Menu::Up()
{
  if(Selected > 0)
  {
    --Selected;
  }
}

void Menu::Down()
{
  if(Selected + 1 < NextItem)
  {
    ++Selected;
  }
}

void Menu::Select()
{
  Items[Selected].OnSelect();
}

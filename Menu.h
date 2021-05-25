#pragma once
#include "Globals.h"

typedef void (*MenuItemSelect)();

enum class WatchType
{
  UINT8,
  UINT16,
  UINT32,
  UINT64,
  FLOAT,
  BOOL,
  PULLUP_PIN
};

struct Watch
{
  const char* Label;
  WatchType   Type;
  void*       ObjectToWatch;
};

struct MenuItem
{
  const char*     Label;
  MenuItemSelect  OnSelect;
};

class Menu
{
  public:
    Menu();
    ~Menu();
    
    void Draw();

    void SetWatchCount(uint8_t Amount);
    void AddWatch(const char* Label,WatchType Type,void* ObjectToWatch);
    void AddWatch(const char* Label,WatchType Type,uint32_t ObjectToWatch);
    
    void SetItemCount(uint8_t Amount);
    void AddItem(const char* Label, MenuItemSelect OnSelect);

    void Up();
    void Down();
    void Select();

  private:
    uint8_t     CurrentStart;
    uint8_t     Selected;
  
    Watch*      Watches;
    uint8_t     NextWatch;
    
    MenuItem*   Items;
    uint8_t     NextItem;
};

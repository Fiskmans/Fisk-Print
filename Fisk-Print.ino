
#include "Globals.h"
#include "Ui.h"
#include "MenuStack.h"
#include "RotationEncoder.h"
#include "RotationEncoderNoButton.h"
#include "Sampler.h"
#include "State.h"
#include "CounterInterface.h"
#include <SD.h>
#include "SerialHelp.h"
#include "Tracker.h"

bool ModifyMode = false;
uint32_t* Target = nullptr;

float Distance = 0;

RotationEncoder* InputWheel;
RotationEncoderNoButton* SecondaryInputWheel;
Tracker* tracker;


void TrackerUp()
{
  tracker->StepBackward();
}

void TrackerDown()
{
  tracker->StepForward();
}

void Up()
{
  if(ModifyMode)
  {
    (*Target)--;
    return;
  }
  MenuStack::Top().Up();
}

void Down()
{
  if(ModifyMode)
  {
    (*Target)++;
    return;
  }
  MenuStack::Top().Down();
}

void Select()
{
  if(ModifyMode)
  {
    MenuStack::Pop();
    ModifyMode = false;
    return;
  }
  MenuStack::Top().Select();
}

void OpenMainMenu()
{
  Menu* menu = new Menu();
  
  menu->SetWatchCount(1);
  menu->AddWatch("Extruded", WatchType::FLOAT,&Distance);
  
  menu->SetItemCount(0);

  MenuStack::Push(menu);
}

void setup() 
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  
  DrawUi();
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Loading")) / 2, 10, "Loading");
  u8g2.sendBuffer();
  
  SerialHelp::Setup();
  SerialHelp::RegisterCommands();

  digitalWrite(LEVEL_SHIFTER_ENABLE,HIGH);
  pinMode(LEVEL_SHIFTER_ENABLE,OUTPUT);
  
  delay(200);
  
#if USE_SD_CARD
  if(!SD.begin(2))
  {
    u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("SD - init faled")) / 2, 30, "SD - init faled");
    Serial.println("SD - initialization faled");
    u8g2.sendBuffer();
    while(true);
  }
#endif
  
  State::Load();
  OpenMainMenu();
  
  InputWheel = new RotationEncoder(27, 14, 12, &Up, &Down, &Select, 500, 80);
  SecondaryInputWheel = new RotationEncoderNoButton(34, 35,&TrackerUp, &TrackerDown);
  Counter = new CounterInterface(EXTERNAL_COUNTER_PIN,State::Values.CounterValue,0);
  tracker = new Tracker("/trackerData.dat");
  
  Serial.println("Initialization complete");
}

void Draw()
{
  u8g2.clearBuffer();
  DrawUi();
  //Clock.Draw();
  //Data.Draw();
  //Sw.Draw();
  MenuStack::Top().Draw();

  if(digitalRead(27))
    u8g2.drawPixel(SCREEN_WIDTH - 5,42);
  if(digitalRead(14))
    u8g2.drawPixel(SCREEN_WIDTH - 5,44);
  if(digitalRead(12))
    u8g2.drawPixel(SCREEN_WIDTH - 5,46);
    
  if(digitalRead(34))
    u8g2.drawPixel(SCREEN_WIDTH - 8,44);
  if(digitalRead(35))
    u8g2.drawPixel(SCREEN_WIDTH - 8,46);
  
  u8g2.sendBuffer();
}

void loop() 
{
  SerialHelp::Update();
  InputWheel->Update();
  SecondaryInputWheel->Update();
  Counter->Update();
  tracker->Update();

  Distance = tracker->GetDistance();

  unsigned long now = millis();
  
  if(State::WantSave && State::LastSave + 500 < now)
  {
    State::Save();
    State::LastSave = now;
    State::WantSave = false;
  }
  
  while(LastFrame + MillisPerFrame < now)
  {
    Draw();
    LastFrame += MillisPerFrame;
  }
}

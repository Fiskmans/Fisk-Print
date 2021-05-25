#include "Tracker.h"
#include <SD.h>

Tracker::Tracker(const char* FileName)
{
  FilePath = FileName;
  
  File file = SD.open(FileName,FILE_READ);
  if(!file)
  {
    Serial.println("TRACKER FAILURE - FAILED TO OPEN FILE");
    return;
  }
  
  uint32_t len = file.available();
  if(len < sizeof(uint32_t))
  {
    Serial.print("Tracker file too small: ");
    Serial.println(len);
    return;
  }
  
  if(len > sizeof(Data))
  {
    Serial.println("State file too large");
    len = sizeof(Data);
  }
  
  uint8_t* data = new uint8_t[len];
  file.read(data,len);
  file.close();

  if(*reinterpret_cast<uint32_t*>(data) != TRACKER_VERSION)
  {
    Serial.print("Tracker file too outdated: ");
    Serial.println(*reinterpret_cast<uint32_t*>(data));
    delete[] data;
    return;
  }

  memcpy(&Data,data,len);

  delete[] data;
}
  
void Tracker::Update()
{
  unsigned long now = millis();
  if(IsDirty && now - LastSave > 500)
  {
    File file = SD.open(FilePath,FILE_WRITE);
    file.write(reinterpret_cast<uint8_t*>(&Data), sizeof(Data));
    file.close();

    LastSave = now;
    IsDirty = false;
  }
}

void Tracker::StepForward()
{
  Data.Distance += Data.StepSize;
  IsDirty = true;
}

void Tracker::StepBackward()
{
  Data.Distance -= Data.StepSize;
  IsDirty = true;
}

float Tracker::GetDistance()
{
  return Data.Distance;
}

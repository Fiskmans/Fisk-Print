#include "State.h"
#include <SD.h>

StateValues   State::Values;
bool          State::WantSave = false;
unsigned long State::LastSave = 0;
const char*   FileName = "/State.dat";

namespace Converters
{
  #define CONVERTER_LOAD(Type1)           \
    if(len < sizeof(Type1))               \
    {                                     \
      Serial.println("Missing Data");     \
      return StateValues();               \
    }                                     \
    uint32_t toRead = len;                \
    if(toRead > sizeof(Type1))            \
    {                                     \
      toRead = sizeof(Type1);             \
    }                                     \
    Type1 original;                       \
    memcpy(&original,data,toRead);        
  
  
  struct V2
  {
    uint32_t  VersionNumber       = 2;
    float     DistancePerRotation = 3;
  };

  struct V1
  {
    uint32_t  VersionNumber       = 1;
    float     DistancePerRotation = 3;
  };

  StateValues ConvertV2(V2 original)
  {
    Serial.println("Upgraded state from V2 to Current");

    StateValues out;
    out.VersionNumber = 3;
    out.DistancePerRotation = original.DistancePerRotation;
    out.CounterValue = 0;
    return out;
  }
  
  StateValues UpgradeFromV2(uint8_t* data, uint32_t len)
  {
    CONVERTER_LOAD(V2);
    return ConvertV2(original);
  }

  StateValues ConvertV1(V1 original)
  {
    Serial.println("Upgraded state from V1 to V2");
    
    V2 out;
    out.VersionNumber = 2;
    out.DistancePerRotation = original.DistancePerRotation;
    return ConvertV2(out);
  }
  
  StateValues UpgradeFromV1(uint8_t* data, uint32_t len)
  {
    CONVERTER_LOAD(V1);
    return ConvertV1(original);
  }
}

void State::Save()
{
#if !USE_SD_CARD
  return;
#endif
  File file = SD.open(FileName,FILE_WRITE);
  file.write(reinterpret_cast<uint8_t*>(&Values), sizeof(Values));
  file.close();
}

void State::Load()
{
#if !USE_SD_CARD
  return;
#endif
  if(SD.exists(FileName))
  {
    Serial.println("SUCCESS - FILE EXISTS");

    File file = SD.open(FileName,FILE_READ);
    if(!file)
    {
      Serial.println("FAILURE - FAILED TO OPEN FILE");
      return;
    }

    uint32_t len = file.available();
    if(len < sizeof(uint32_t))
    {
      Serial.print("State file too small: ");
      Serial.println(len);
      return;
    }
    
    if(len > sizeof(Values))
    {
      Serial.println("State file too large");
      len = sizeof(Values);
    }
    
    uint8_t* data = new uint8_t[len];
    file.read(data,len);
    file.close();
    
    uint32_t fileVersion = *reinterpret_cast<uint32_t*>(data);
    if(fileVersion == STATE_VERSION_NUMBER)
    {
      memcpy(&Values,data,len);
      Serial.println("State file loaded successfully");
    }
    else
    {
      switch(fileVersion)
      {
        case 1:
          Values = Converters::UpgradeFromV1(data,len);
          break;
        case 2:
          Values = Converters::UpgradeFromV2(data,len);
          break;
        default:
          Serial.print("State file is outdated, and there are no available converters for version: ");
          Serial.println(fileVersion);
          break;
      }
    }
  }
  else
  {
    Serial.println("State file doesn't exist leaving state defaultConstructed and saving");
    Save();
  }
}

#include "SerialCommands.h"
#include <SD.h>
#include "CounterInterface.h"

namespace Commands
{
#if USE_SD_CARD
  char CurrentDir[MAX_PATH_LENGTH+1] = "";
#endif

  void echo(ArgumentParser& args)
  {
    uint8_t i = 1;

#if USE_SD_CARD
    args.PrintAll();

    const char* file = args.Get("file","f");
    if(file)
    {
      Serial.println(file);
      char fullPath[MAX_PATH_LENGTH+1];
      int len = sprintf(fullPath,"%s/%s",CurrentDir, file);
      File f = SD.open(fullPath,FILE_WRITE);
      Serial.print("Echo >> ");
      Serial.println(fullPath);
      while(true)
      {
        const char* arg = args.Get(i++);
        if(!arg)
        {
          break;
        }
        f.print(arg);
      }
      f.close();
      return;
    }
#endif
    
    while(true)
    {
      const char* arg = args.Get(i++);
      if(!arg)
      {
        break;
      }
      Serial.print(arg);
      Serial.print(" ");
    }
    Serial.println("");
  }

#if USE_SD_CARD

  void dumpfile(ArgumentParser& args)
  {
    Serial.print("Dumping file: ");
    const char* filePath = args.Get("file", "f", 1);
    if(filePath)
    {
      char fullPath[MAX_PATH_LENGTH] = "";
      sprintf(fullPath, "%s/%s", CurrentDir, filePath);
      
      Serial.println(fullPath);
      if(!SD.exists(fullPath))
      {
        throw "No such file on sd card";
      }
      File file = SD.open(fullPath,FILE_READ);
      
      if(args.Get("binary","b"))
      {
        uint32_t  at = 0;
        uint8_t   buffer[16];
        char      printBuffer[128];
        
        while(file.available() > 0)
        {
          uint8_t len = file.available() > 16 ? 16 : file.available();
          file.read(buffer,len);

          sprintf(printBuffer,"0x%05x:  ",at);
          Serial.print(printBuffer);
          for(uint8_t i = 0; i < len; i++)
          {
            sprintf(printBuffer,"0x%02x ",buffer[i]);
            Serial.print(printBuffer);
          }
          Serial.println("");
        }
      }
      else
      {
        uint8_t   buffer[16];
        while(file.available() > 0)
        {
          uint8_t len = file.available() > 16 ? 16 : file.available();
          file.read(buffer,len);
          Serial.write(buffer,len);
        }
      }
      file.close();
    }
    else
    {
      Serial.println("No file argument");
    }
  }
  
  void ls(ArgumentParser& args)
  {
    File f = SD.open((*CurrentDir) == '\0' ? "/" : CurrentDir);
    File entry;
    while(entry = f.openNextFile())
    {
      Serial.print(entry.name());
      if (entry.isDirectory())
      {
        Serial.println("/");
      }
      else
      {
        Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
      }
      entry.close();
    }
    f.close();
  }
  
  void cd(ArgumentParser& args)
  {
    const char* name = args.Get("file", "f", 1);
    if(!name)
    {
      Serial.println((*CurrentDir) == '\0' ? "/" : CurrentDir);
      return;
    }
    if(strcmp("..",name) == 0)
    {
      char* last = CurrentDir;
      char* end = last;
      while((*end) != '\0')
      {
        if(*end == '/')
        {
          last = end;
        }
        end++;
      }
      *last = '\0';
      return;
    }
    
    char tmp[MAX_PATH_LENGTH+1];
    int len = sprintf(tmp,"%s/%s",CurrentDir, name);
    if(len <= MAX_PATH_LENGTH+1)
    {
      if(SD.exists(tmp))
      {
        File f = SD.open(tmp);
        if(f.isDirectory())
        {
          memcpy(CurrentDir,tmp,MAX_PATH_LENGTH+1);
        }
        else
        {
          Serial.println("Target is a file");  
        }
        f.close();
      }
      else
      {
        Serial.print("Target does not exist: ");
        Serial.println(tmp);
      }
    }
    else
    {
      throw "Path is too long";
    }
  }
  
  void mkdir(ArgumentParser& args)
  {
    const char* name = args.Get("dir", "d", 1);
    if(!name)
    {
      throw "Missing dir, d or second argument";
    }
    char tmp[MAX_PATH_LENGTH+1];
    int len = sprintf(tmp,"%s/%s",CurrentDir, name);
    SD.mkdir(tmp);
  }
  
  void mkfile(ArgumentParser& args)
  {
    const char* name = args.Get("file", "f", 1);
    if(!name)
    {
      throw "Missing file, f or second argument";
    }
    char tmp[MAX_PATH_LENGTH+1];
    int len = sprintf(tmp,"%s/%s",CurrentDir, name);
    SD.open(tmp,FILE_WRITE).close();
  }
#endif

  void counter(ArgumentParser& args)
  {
    if(args.Get("reset","r"))
    {
      Counter->SetCurrent(0);
      Counter->SetTarget(0);
    }
    
    const char* current = args.Get("current","c",2);
    uint32_t number;
    if(ArgumentParser::AsNumber(current,number))
    {
      Serial.print("Set current: ");
      Serial.println(number);
      Counter->SetCurrent(number);
    }
    
    const char* target = args.Get("target","t",3);
    if(ArgumentParser::AsNumber(target,number))
    {
      Serial.print("Set target: ");
      Serial.println(number);
      Counter->SetTarget(number);
    }
    
    const char* add = args.Get("add","a",1);
    if(ArgumentParser::AsNumber(add,number))
    {
      Serial.print("added to target: ");
      Serial.println(number);
      Counter->Add(number);
    }
    
    Counter->PrintStatus();
  }
}

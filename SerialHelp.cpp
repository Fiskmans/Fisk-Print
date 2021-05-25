#pragma once
#include "SerialHelp.h"
#include "SerialCommands.h"

SerialHelp::Callback  SerialHelp::CallBacks[MAX_COMMANDS];
uint32_t              SerialHelp::CallBackCount;
    
char                  SerialHelp::CommandBuffer[MAX_COMMAND_LENGTH + 1];
uint32_t              SerialHelp::WriteOffset;
bool                  SerialHelp::CommandRejected;


ArgumentParser::ArgumentParser(char** argv,int argc)
{
  RawArgsCount    = 0;
  NamedArgsCount  = 0;
  
  bool NextIsNamed = false;

  for(uint8_t i = 0; i < argc; i++)
  {
    char* arg = argv[i];
    
    if(*arg == COMMAND_NAMED_ARG_CHAR)
    {
      NamedArgs[NamedArgsCount].Name = arg + 1;
      NamedArgs[NamedArgsCount++].Arg = "true";
      NextIsNamed = true;
    }
    else
    {
      if(NextIsNamed)
      {
        NamedArgs[NamedArgsCount - 1].Arg = arg;
        NextIsNamed = false;
      }
      else
      {
        RawArgs[RawArgsCount++] = arg;
      }
    }
  }
}

const char* ArgumentParser::Get(uint8_t Number)
{
  if(Number < RawArgsCount)
  {
    return RawArgs[Number];
  }
  return nullptr;
}

const char* ArgumentParser::Get(const char* Name)
{
  
  for(uint8_t i = 0; i < NamedArgsCount; i++)
  {
    if(strcmp(NamedArgs[i].Name, Name) == 0)
    {
      return NamedArgs[i].Arg;
    }
  }
  return nullptr;
}

const char* ArgumentParser::Get(const char* Name,uint8_t Number)
{
  const char* out = Get(Name);
  if(!out)
  {
    return Get(Number);
  }
  return out;
}

const char* ArgumentParser::Get(const char* Name1, const char* Name2)
{
  const char* out = Get(Name1);
  if(!out)
  {
    return Get(Name2);
  }
  return out;
}

const char* ArgumentParser::Get(const char* Name1, const char* Name2, uint8_t Number)
{  
  const char* out = Get(Name1);
  if(!out)
  {
    return Get(Name2, Number);
  }
  return out;
}

bool ArgumentParser::AsNumber(const char* raw,uint32_t& out)
{
  if(!raw)
  {
    return false;
  }
  uint32_t val = 0;
  const char* at = raw;
  while(*at != '\0')
  {
    char c = *at;
    if(c < '0' || c > '9')
    {
      return false;
    }
    val *= 10;
    val += c - '0';
    at++;
  }
  out = val;
  return true;
}

void ArgumentParser::PrintAll()
{
  Serial.println("Unamed:");
  for(uint8_t i = 0; i < RawArgsCount; i++)
  {
    Serial.print("\t");
    Serial.print(i);
    Serial.print(":\t");
    Serial.println(RawArgs[i]);
  }
  
  Serial.println("Named:");
  for(uint8_t i = 0; i < NamedArgsCount; i++)
  {
    Serial.print("\t");
    Serial.print(NamedArgs[i].Name);
    Serial.print(":\t");
    Serial.println(NamedArgs[i].Arg);
  }
}

void SerialHelp::Setup()
{
  CallBackCount = 0;
  WriteOffset = 0;
  memset(CommandBuffer,0,sizeof(CommandBuffer));
  
  Serial.begin(SERIAL_BAUD_RATE);
  while(!Serial);
  Serial.println("");
  Serial.println("Begin");
}

void SerialHelp::RegisterCommands()
{
  RegisterCommand("echo",&Commands::echo);
#if USE_SD_CARD
  RegisterCommand("dumpfile",&Commands::dumpfile);
  RegisterCommand("ls",&Commands::ls);
  RegisterCommand("dir",&Commands::ls);
  RegisterCommand("cd",&Commands::cd);
  RegisterCommand("mkdir",&Commands::mkdir);
  RegisterCommand("mkfile",&Commands::mkfile);
#endif
  RegisterCommand("counter",&Commands::counter);
}

void SerialHelp::RegisterCommand(const char* Name, CommandFunction Callback)
{
  CallBacks[CallBackCount].Name = Name;
  CallBacks[CallBackCount++].Callback = Callback;
}

void SerialHelp::Update()
{
  while(Serial.available() > 0)
  {
    while(Serial.available() > 0)
    {
      char c = static_cast<char>(Serial.read());
      if(c == COMMAND_END_CHAR)
      {
        Process();
      }
      else
      {
        if(WriteOffset < MAX_COMMAND_LENGTH)
        {
          CommandBuffer[WriteOffset++] = c;
        }
        else
        {
          Reject();
        }
      }
    }
  }
}

void SerialHelp::PrepareBuffer(char* argv[],int& argc)
{
  argc = 0;
  char* begin = CommandBuffer;
  char* end = begin;
  
  bool isQuoted = false;
  while(*(end) != '\0')
  {
    if(*end == '"')
    {
      if(isQuoted)
      {
        if(argc == MAX_COMMAND_ARGS)
        {
          throw "Too many arguments";
        }
        isQuoted = false;
        argv[argc++] = begin + 1;
        *end = '\0';
        begin = end + 1;
      }
      else
      {
        isQuoted = true;
      }
    }

    if(!isQuoted && *end == ' ')
    {
      if(end - begin > 1)
      {
        if(argc == MAX_COMMAND_ARGS)
        {
          throw "Too many arguments";
        }
        argv[argc++] = begin;
        *end = '\0';
      }
      begin = end + 1;
    }

    end++;
  }
  if(end - begin > 1)
  {
    if(argc == MAX_COMMAND_ARGS)
    {
      throw "Too many arguments";
    }
    argv[argc++] = begin;
    *end = '\0';
  }
}

void SerialHelp::Process()
{
  CommandBuffer[WriteOffset] = '\0';
  WriteOffset = 0;
  CommandRejected = false;
  
  if(CommandRejected)
  {
    Serial.println("Command rejected");
  }
  else
  {

    Serial.print("Got command: ");
    Serial.println(CommandBuffer);

    char* argv[MAX_COMMAND_ARGS];
    int argc = 0;

    try
    {
      PrepareBuffer(argv,argc);

      if(argc == 0)
      {
        throw "No arguments or commands parsed";
      }

      for(uint8_t i = 0; i < CallBackCount; i++)
      {
        if(strcmp(CallBacks[i].Name,*argv) == 0)
        {
          ArgumentParser args(argv,argc);
          CallBacks[i].Callback(args);
          return;
        }
      }
      
      Serial.print("No command found matching: ");
      Serial.println(*argv);
    }
    catch (const char* e)
    {
        Serial.print("Command crashed: ");
        Serial.println(e);
    }
  }
}

void SerialHelp::Reject()
{
  WriteOffset = 0;
  CommandRejected = true;
}

#pragma once
#include "Globals.h"

#define MAX_COMMAND_LENGTH      (1024)
#define MAX_COMMAND_ARGS        (16)
#define MAX_COMMANDS            (32)

#define COMMAND_END_CHAR        ('\n')
#define COMMAND_ARG_CHAR        (' ')
#define COMMAND_NAMED_ARG_CHAR  ('-')

#define SERIAL_BAUD_RATE        (115200)

class ArgumentParser
{
  public:
    ArgumentParser(char** argv,int argc);
    
    const char* Get(uint8_t Number);
    const char* Get(const char* Name);
    const char* Get(const char* Name,uint8_t Number);
    const char* Get(const char* Name1, const char* Name2);
    const char* Get(const char* Name1, const char* Name2, uint8_t Number);

    static bool AsNumber(const char* raw,uint32_t& out);

    void PrintAll();
  private:
    struct NamedArg
    {
      const char* Name;
      const char* Arg;
    };
    
    const char* RawArgs[MAX_COMMAND_ARGS];
    uint8_t     RawArgsCount;

    NamedArg    NamedArgs[MAX_COMMAND_ARGS/2];
    uint8_t     NamedArgsCount;
};


typedef void (*CommandFunction)(ArgumentParser& args);


class SerialHelp
{
  public:
    static void Setup();
    static void RegisterCommands();
    static void RegisterCommand(const char* Name, CommandFunction Callback);
    
    static void Update();

  private:
  
    struct Callback
    {
      const char*     Name;
      CommandFunction Callback;
    };
    
    static void Process();
    static void Reject();
    
    static void PrepareBuffer(char* argv[],int& argc);

    static Callback  CallBacks[MAX_COMMANDS];
    static uint32_t  CallBackCount;
    
    static char      CommandBuffer[MAX_COMMAND_LENGTH + 1];
    static uint32_t  WriteOffset;
    static bool      CommandRejected;
};

#pragma once
#include "Globals.h"
#include "SerialHelp.h"

#define MAX_PATH_LENGTH (256)

namespace Commands
{  
  void echo(ArgumentParser& args);
#if USE_SD_CARD
  void dumpfile(ArgumentParser& args);
  void ls(ArgumentParser& args);
  void cd(ArgumentParser& args);
  void mkdir(ArgumentParser& args);
  void mkfile(ArgumentParser& args);
#endif

  void counter(ArgumentParser& args);
}

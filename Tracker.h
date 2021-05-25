#include "Globals.h"

#define TRACKER_VERSION (1)

class Tracker
{
  struct TrackerData
  {
    uint32_t Version = TRACKER_VERSION;
    float StepSize = 3.4f/32.f / 100.f;
    float Distance = 0;
  };
public:
  Tracker(const char* FileName);
  
  void Update();

  void StepForward();
  void StepBackward();

  float GetDistance();

private:
  TrackerData Data;
  bool IsDirty;
  unsigned long LastSave = 0;
  const char* FilePath;
};

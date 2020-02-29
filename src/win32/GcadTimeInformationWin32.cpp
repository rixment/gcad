#include "GcadTimeInformationWin32.h"
#include <windows.h>

namespace Gcad {
namespace Platform {

size_t TimeInformationWin32
::getSystemTime() const
{ 
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  return ticks.LowPart;
}   

size_t TimeInformationWin32
::getTicksCountPerSec() const
{
  LARGE_INTEGER ticksPerSec;
  QueryPerformanceFrequency(&ticksPerSec);
  return ticksPerSec.LowPart;
}

} // namespace Platform
} // namespace Gcad
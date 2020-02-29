#ifndef _GCAD_TIMEINFORMATIONWIN32_H_
#define _GCAD_TIMEINFORMATIONWIN32_H_

#include "GcadPlatformWin32Base.h"
#include "GcadTimeInformation.h"

namespace Gcad {
namespace Platform {

/**
  @brief
    Realizacja interfejsu dla systemu Microsoft Windows 
    architektury 32 bitowej
*/
class GCAD_WIN32_EXPORT TimeInformationWin32 : public TimeInformation {
 public:
   virtual size_t getSystemTime() const;
   virtual size_t getTicksCountPerSec() const;
};

} // namespace Platform
} // namespace Gcad

#endif
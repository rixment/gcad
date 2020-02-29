#ifndef _GCAD_DRIVEELEMENTSENUMERATORWIN32_H_
#define _GCAD_DRIVEELEMENTSENUMERATORWIN32_H_

#include "GcadPlatformWin32Base.h"
#include "GcadDriveElementsEnumerator.h"
#include <windows.h>

namespace Gcad {
namespace Platform {

/**
  @brief
    Realizacja interfejsu wykonana dla platformy Win32
*/
class GCAD_WIN32_EXPORT DriveElementsEnumeratorWin32 
  : public DriveElementsEnumerator 
{
 public:
   /**
     @brief
       Realizacja klasy interfejsu iteratora elementow 
       struktury hierarchicznej plik/katalog
   */
   class GCAD_WIN32_EXPORT Itor 
     : public DriveElementsEnumerator::Itor
   {
    private:
      friend class DriveElementsEnumeratorWin32;
      
      Itor(const DriveElementsEnumeratorWin32& owner);
    
    public:      
      ~Itor();

      virtual bool isDirectory() const;

      virtual std::string getElementName() const;

      virtual bool moveToNextElement();

      virtual bool isValid() const;

      virtual void moveToBegin();
    
    private:
      const DriveElementsEnumeratorWin32&  owner_;
      HANDLE                               currDir_;   
      WIN32_FIND_DATA                      data_;
   };
   
   friend class Itor;

   DriveElementsEnumeratorWin32(const std::string& directory = ".");
   
   virtual DriveElementsEnumeratorWin32* clone() const;

   virtual ItorAutoPtr getItor() const;

   virtual void setDirectory(const std::string& newDirectory);
   
 private:
   std::string  baseDir_;
};

} // namespace Platform
} // namespace Gcad

#endif
/***************************************************************************
 *   Copyright (C) 2005 by Eryk Klebanski                                  *
 *   rixment@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
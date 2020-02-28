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
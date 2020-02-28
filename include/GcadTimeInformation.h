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

#ifndef _GCAD_TIMEINFORMATION_H_
#define _GCAD_TIMEINFORMATION_H_

#include "GcadBase.h"
#include <cstddef>

namespace Gcad {
namespace Platform {

/**
  @brief
    Interfejs umozliwiajacy pobranie wartosci informujacej o biezacym
    czasie systemowym

  @remark
    Na jego podstawie opiera sie kryterium sortowania posrednikow zasobow 
    wewnetrznej reprezentacji menadzera DataFileResourceManager
*/
class GCAD_EXPORT TimeInformation {
 public:
   virtual ~TimeInformation();
      
   /**
     @brief
       Uzyskanie wartosci determinujacej biezacy czas systemowy
   */
   virtual size_t getSystemTime() const = 0;

   /**
     @brief
       Uzyskanie wartosci determinujacej ilosc taktow zegara 
       wykonanych w ciagu jednej sekundy
   */
   virtual size_t getTicksCountPerSec() const = 0;
};

} // namespace Platform
} // namespace Gcad

#endif

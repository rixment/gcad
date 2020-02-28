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

#ifndef _GCAD_IMOUSE_H_
#define _GCAD_IMOUSE_H_

#include "GcadBase.h"

namespace Gcad {
namespace Platform {

/** 
  @brief
    Interfejs nalezacy do podsystemu wejscia, definiujacy specyficzne
    zachowania dla urzadzenia myszy
*/
class GCAD_EXPORT Mouse {
 public:
   virtual ~Mouse();

   virtual long xAxis() = 0;
   virtual long yAxis() = 0;
   virtual long zAxis() = 0;

   /** 
     @brief Sprawdza aktualny stan przyciskow myszki
     @param buttonNum Wartosc okreslajaca przycisk myszy z przedzialu
       od 0 do 7
   */
   virtual bool button( int buttonNum ) = 0;
};

} // namespace Platform
} // namespace Gcad

#endif

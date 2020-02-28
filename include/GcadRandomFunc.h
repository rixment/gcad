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

#ifndef _GCAD_RANDOMFUNC_H_
#define _GCAD_RANDOMFUNC_H_

#include "GcadBase.h"

namespace Gcad {
namespace Math {

/** 
  @brief
    Zbior funkcji generatora liczb pseudolosowych
*/
struct GCAD_EXPORT RandomFunc {
  /** 
    @brief Wykorzystywana do utworzenia tablicy szumu 
  */
  static double Noise2D( int x, int y );
  
  /** 
    @brief Wykorzystywana do utworzenia wolumetrycznego szumu 
  */
  static double Noise3D( int x, int y, int z );
};

} // namespace Math
} // namespace Gcad

#endif
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

#include "GcadRandomFunc.h"

namespace Gcad {
namespace Math {

//
double RandomFunc
::Noise2D( int x, int y )
{
  int n = x + y * 57;
  n = ( n << 13 ) ^ n;
  return
    ( 1.0 - ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7FFFFFFF ) / 
    1073741824.0 );
}

//
double RandomFunc
::Noise3D( int x, int y, int z )
{
  int n = x + y * 57 + z * 131;
  n = ( n << 13 ) ^ n;
  return 
    ( 1.0 - ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7FFFFFFF ) * 
    0.000000000931322574615478515625 );
}

} // namespace Math
} // namespace Gcad

// zrodlo: Programowanie Gier w DirectX. M. McCuskey. str.543
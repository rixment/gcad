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

#ifndef _GCAD_MATHFWD_H
#define _GCAD_MATHFWD_H

namespace Gcad {
namespace Math {

  template<typename REAL> class Cylindrical3;
  template<typename REAL> class LinearInterpolation;
  template<typename REAL> class ParamLine2;
  template<typename REAL> class ParamLine3;
  template<typename REAL> class Plane;
  template<typename REAL> class Polar2;
  template<typename REAL> class Spherical3;
  template<typename REAL> class Vector2;
  template<typename REAL> class Vector3;
  
  template<int ROWS, int COLS, typename T> class Matrix;

} // namespace Math
} // namespace Gcad

#endif
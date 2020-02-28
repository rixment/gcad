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

#ifndef _GCAD_PARAMLINE3UTIL_H_
#define _GCAD_PARAMLINE3UTIL_H_

#include "GcadParamLine3.h"
#include "GcadVector3Util.h"

namespace Gcad {
namespace Math {

/**
  @brief
    Zestaw funkcji uzupelniajacych funkcjonalnosc 
    trojwymiarowej parametrycznej prostej 
*/
struct ParamLine3Util {  
   
  /**
    @brief Oblicza dlugosc odcinka 3d 
  */
  template< typename REAL >
  static REAL 
    length( const ParamLine3<REAL>&  line );
  
  /**
    @brief Zwraca wierzcholek konczacy odcinek
  */
  template< typename REAL >
  static Vector3<REAL>
    endPoint( const ParamLine3<REAL>&  line );

  /** 
    @brief 
      Oblicza punkt lezacy na prostej wyznaczanej 
      przez parametr line, oraz t
  */
  template< typename REAL >
  static Vector3<REAL> 
    computePoint( const ParamLine3<REAL>&  line, 
                  REAL                     t ); 
 
 private:
   // Nie zaimplementowane
   ParamLine3Util();
   ParamLine3Util( const ParamLine3Util& );
   ParamLine3Util& operator =( const ParamLine3Util& );

};


template< typename REAL >
REAL 
ParamLine3Util
::length( const ParamLine3<REAL>&  line ) 
{
  return Vector3Util::length( line.v() );
}

template< typename REAL >
Vector3<REAL> 
ParamLine3Util
::endPoint( const ParamLine3<REAL>& line ) 
{
  return line.p0() + line.v();
}

template< typename REAL >
Vector3<REAL>
ParamLine3Util
::computePoint( const ParamLine3<REAL>&  line, 
                REAL                     t )
{
  return line.p0() + line.v() * t;
}


} // namespace Math
} // namespace Gcad

#endif
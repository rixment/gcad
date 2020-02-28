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

#ifndef _GCAD_COORD2UTIL_H_
#define _GCAD_COORD2UTIL_H_

#include "GcadAssertion.h"
#include "GcadPolar2.h"
#include "GcadVector2.h"
#include "GcadVector2Util.h"

#include <cmath>

namespace Gcad {
namespace Math {

/**
  @brief
    Zestaw funkcji do przeprowadzania standardowych konwersji 
    miedzy ukladami dwuwymiarowymi
*/
struct Coord2Util { 
  /** 
    @brief 
      Transformacja wartosci wspolrzednych z ukladu kartezjanskiego 
      na biegunowy
    
    @param 
      u Wspolrzedne poddawane przeksztalceniu

    @param 
      out Obiekt docelowy, w ktorym zostana zachowane przeksztalcone 
      wspolrzedne
  */
  template<typename REAL>
  static void Vec2ToPolar2( const Vector2<REAL>&  u, 
                            Polar2<REAL>*         out );

  /** 
    @brief 
      Transformacja wspolrzednych z ukladu biegunowego na kartezjanski
    
    @param 
      p Wspolrzedne biegunowe poddawane transformacji
    
    @param 
      out Obiekt docelowy, w ktorym zostana zachowane przeksztalcone
      wspolrzedne
  */
  template<typename REAL>
  static void Polar2ToVec2( const Polar2<REAL>&  p, 
                            Vector2<REAL>*       out );

 private:
   // Nie zaimplementowane
   Coord2Util();
   Coord2Util( const Coord2Util& );
   Coord2Util& operator =( const Coord2Util& );
};


// polar.r     = sqrt( vector.x^2 + vector.y^2 ) = vector.length
// polar.theta = atan( vector.y / vector.x )
template<typename REAL>
void 
Coord2Util
::Vec2ToPolar2( const Vector2<REAL>&  u,
                Polar2<REAL>*         out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  const REAL  NEAR_ZERO = 1e-5;
  const REAL  x         = u.x() == static_cast<REAL>(0) ? NEAR_ZERO : u.x();

  out->setR( Vector2Util::length( u ) );
  out->setTheta( static_cast<REAL>( std::atan( u.y() / x ) ) );
}

// vector.x = polar.r * cos( polar.theta )
// vector.y = polar.r * sin( polar.theta )
template<typename REAL>
void 
Coord2Util
::Polar2ToVec2( const Polar2<REAL>&  p,
                Vector2<REAL>*       out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( p.r() * static_cast<REAL>( std::cos( p.theta() ) ) );
  out->setY( p.r() * static_cast<REAL>( std::sin( p.theta() ) ) );
}


} // namespace Math
} // namespace Gcad

#endif
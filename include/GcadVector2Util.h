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

#ifndef _GCAD_VECTOR2UTIL_H_
#define _GCAD_VECTOR2UTIL_H_

#include "GcadAssertion.h"
#include "GcadVector2.h"

#include <cmath>

namespace Gcad {
namespace Math {

/** 
  @brief
    Zestaw podstawowych operacji wykonywanych na wektorach dwu-wymiarowych
*/
struct Vector2Util {
  
  /** 
    @brief Suma wektorow 
  */
  template<typename REAL>
  static void add( const Vector2<REAL>&  u, 
                   const Vector2<REAL>&  v, 
                   Vector2<REAL>*        out );

  /** 
    @brief Roznica wektorow 
  */
  template<typename REAL>
  static void sub( const Vector2<REAL>&  u, 
                   const Vector2<REAL>&  v, 
                   Vector2<REAL>*        out );

  /** 
    @brief Mnozenie wektora przez skalar 
  */
  template<typename REAL>
  static void mul( const Vector2<REAL>&  u, 
                   REAL                  scalar, 
                   Vector2<REAL>*        out ) ;
  
  /** 
    @brief Dzielenie wektora przez skalar 
  */
  template<typename REAL>
  static void div( const Vector2<REAL>&  u, 
                   REAL                  scalar, 
                   Vector2<REAL>*        out );

  /** 
    @brief Obliczenie wektora przeciwnego 
  */
  template<typename REAL>
  static void invert( const Vector2<REAL>&  u, 
                      Vector2<REAL>*        out ); 

  /**
    @brief Iloczyn skalarny wektorow 
  */
  template<typename REAL>
  static REAL dot( const Vector2<REAL>&  u, 
                   const Vector2<REAL>&  v );

  /** 
    @brief Obliczenie dlugosci wektora 
  */
  template<typename REAL>
  static REAL length( const Vector2<REAL>& v );

  /** 
    @brief Normalizacja wektora 
  */
  template<typename REAL>
  static void normalize( const Vector2<REAL>&  v,
                         Vector2<REAL>*        out );

  /**
    @brief Obliczenie dystansu miedzy zadanymi wektorami
  */
  template< typename REAL >
  static REAL distance( const Vector2<REAL>&  v1,
                        const Vector2<REAL>&  v2 );

  /**
    @brief Wykonanie sklaowania wzgledem zadanego czynnika, oraz osi odniesienia
  */
  template< typename REAL >
  static Vector2<REAL> axisScale(const Vector2<REAL>& axis,
                                 REAL scaleFactor,
                                 const Vector2<REAL>& vector);

  /**
    @brief Wykonanie zmiany ukladu wektora wzgledem drugiego wektora
  */
  template< typename REAL >
  static Vector2<REAL> coordSystemTransform( 
    const Vector2<REAL>& coordSystemAxis,
    const Vector2<REAL>& projectedVector );

 private:
   // nie zaimplementowane
   Vector2Util();
   Vector2Util( const Vector2Util& );
   Vector2Util& operator =( const Vector2Util& );

};


//
template<typename REAL> 
void 
Vector2Util
::add( const Vector2<REAL>&  u,
       const Vector2<REAL>&  v,
       Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.x() + v.x() );
  out->setY( u.y() + v.y() );
}

//
template<typename REAL> 
void 
Vector2Util
::sub( const Vector2<REAL>&  u,
       const Vector2<REAL>&  v,
       Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.x() - v.x() );
  out->setY( u.y() - v.y() );
}

//
template<typename REAL> 
void 
Vector2Util
::mul( const Vector2<REAL>&  u,
       REAL                  scalar,
       Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.x() * scalar );
  out->setY( u.y() * scalar );
}

//
template<typename REAL> 
void 
Vector2Util
::div( const Vector2<REAL>&  u,
       REAL                  scalar,
       Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  if( scalar == static_cast<REAL>(0) ) {
    const REAL NEAR_ZERO = 1e-5;
    scalar = NEAR_ZERO;
  }

  out->setX( u.x() / scalar );
  out->setY( u.y() / scalar );
}

//
template<typename REAL> 
void 
Vector2Util
::invert( const Vector2<REAL>&  u,
          Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( -u.x() );
  out->setY( -u.y() );
}

//
template<typename REAL>
REAL
Vector2Util
::dot( const Vector2<REAL>&  u,
       const Vector2<REAL>&  v )
{
  return u.x() * v.x() + u.y() * v.y();
}

//
template<typename REAL> 
void 
Vector2Util
::normalize( const Vector2<REAL>&  v,
             Vector2<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  div( v, length( v ), out );
}

//
template<typename REAL> 
REAL 
Vector2Util
::length( const Vector2<REAL>& v ) 
{
  REAL C = dot( v, v );
  return static_cast< REAL >( std::sqrt( C ) );
}

//
template< typename REAL >
REAL 
Vector2Util
::distance( const Vector2<REAL>&  v1,
            const Vector2<REAL>&  v2 )
{
  Vector2<REAL> vDist;
  sub(v2, v1, &vDist);
  return length(vDist);
}

//
template< typename REAL >
Vector2<REAL> 
Vector2Util
::axisScale(const Vector2<REAL>& axis,
            REAL scaleFactor,
            const Vector2<REAL>& vector)
{
  Vector2<REAL> parallelVec(axis * dot(vector, axis));
  return scaleFactor * parallelVec +
    vector - parallelVec;
}

//! @todo hmm... ???
template< typename REAL >
Vector2<REAL> 
Vector2Util
::coordSystemTransform( const Vector2<REAL>& coordSystemAxis,
                        const Vector2<REAL>& projectedVector )
{
  Vector2<REAL> projAxis;
  normalize(coordSystemAxis, &projAxis);
  REAL projectedMagnitude = dot(projAxis, projectedVector);
  return Vector2<REAL>(
    projectedMagnitude,
    length(projectedVector - projAxis * projectedMagnitude) 
  );
}

} // namespace Math
} // namespace Gcad

#endif

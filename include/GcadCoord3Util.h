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

#ifndef _GCAD_COORD3UTIL_H_
#define _GCAD_COORD3UTIL_H_

#include "GcadAssertion.h"
#include "GcadCylindrical3.h"
#include "GcadSpherical3.h"
#include "GcadVector3.h"

namespace Gcad {
namespace Math {

/** 
  @brief
    Rodzina funkcji statycznych, umozliwiajacych transformacje wspolrzednych
    pomiedzy ukladami trojwymiarowymi

    Rodzina funkcji statycznych, umozliwiajacych transformacje wspolrzednych
    pomiedzy ukladami trojwymiarowymi:
    - kartezjanskim (prostokatnym)
    - cylindrycznym
    - sferycznym

  @remark 
    Umieszczenie w strukturze funkcji statycznych umozliwia organizacje
    spojnych skladowych, oraz zapobiega przed zanieczyszczaniem przestrzeni
    niepowiazanymi nazwami identyfikatorow
*/
struct Coord3Util {
  /** 
    @brief 
      Transformacja wspolrzednych z ukladu prostokatnego na cylindyczny
  */
  template< typename REAL >
  static void Vec3ToCyl3( const Vector3<REAL>&  vec, 
                          Cylindrical3<REAL>*   out );

  /** 
    @brief 
      Transformacja wspolrzednych z ukladu prostokatnego na sferyczny
  */
  template< typename REAL >
  static void Vec3ToSph3( const Vector3<REAL>&  vec, 
                          Spherical3<REAL>*     out );

  /** 
    @brief 
      Transformacja wspolrzednych z ukladu cylindrycznego na prostokatny
  */
  template< typename REAL >
  static void Cyl3ToVec3( const Cylindrical3<REAL>&  cln, 
                          Vector3<REAL>*             out );

  /** 
    @brief 
      Transformacja wspolrzednych z ukladu cylindycznego na sferyczny
  */
  template< typename REAL >
  static void Cyl3ToSph3( const Cylindrical3<REAL>&  cln, 
                          Spherical3<REAL>*          out );

  /**
    @brief 
      Transformacja wspolrzednych z ukladu sferycznego na prostokatny
  */
  template< typename REAL >
  static void Sph3ToVec3( const Spherical3<REAL>&  sph, 
                          Vector3<REAL>*           out );

  /** 
    @brief 
      Transformacja wspolrzednych z ukladu sferycznego na cylindryczny
  */
  template< typename REAL >
  static void Sph3ToCyl3( const Spherical3<REAL>&  sph,
                          Cylindrical3<REAL>*      out );

 private:
   // Nie zaimplementowane
   Coord3Util();
   Coord3Util( const Coord3Util& );
   Coord3Util& operator =( const Coord3Util& );
};


// cylindrical.r     = sqrt( vector.x^2 + vector.y^2 )
// cylindrical.theta = atan( vector.y / vector.x )
// cylindrical.z     = vector.z
template< typename REAL >
void 
Coord3Util
::Vec3ToCyl3( const Vector3<REAL>&  vec,
              Cylindrical3<REAL>*   out )
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  const REAL  NEAR_ZERO = 1e-5;
  const REAL  x         = vec.x() == static_cast<REAL>(0) ? NEAR_ZERO : vec.x();
  
  out->setR( static_cast< REAL >( std::sqrt( x * x + vec.y() * vec.y() ) ) );
  out->setTheta( static_cast< REAL >( std::atan( vec.y() / x ) ) );
  out->setZ( vec.z() );
}

// spherical.p     = vector.length
// spherical.theta = atan( vector.y / vector.x )
// spherical.pi    = asin( sqrt( vector.x^2 + vector.y^2 ) / spherical.p )
template< typename REAL >
void 
Coord3Util
::Vec3ToSph3( const Vector3<REAL>&  vec,
              Spherical3<REAL>*     out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  const REAL  NEAR_ZERO = 1e-5;
  const REAL  x         = vec.x() == static_cast<REAL>(0) ? NEAR_ZERO : vec.x();
  
  out->setRho( Vector3Util::length( vec ) );
  out->setTheta( static_cast< REAL >( std::atan( vec.y() / x ) ) );
  
  const REAL c = std::sqrt( x * x + vec.y() * vec.y() );
  
  out->setPi( static_cast< REAL >( std::asin( c / out->rho() ) ) );
}

// vector.x =
// vector.y =
// vector.z = cylindrical.z
template< typename REAL >
void 
Coord3Util
::Cyl3ToVec3( const Cylindrical3<REAL>&  cln,
              Vector3<REAL>*             out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );
  
  out->setX( static_cast< REAL >( cln.r() * std::cos( cln.theta() ) ) );
  out->setY( static_cast< REAL >( cln.r() * std::sin( cln.theta() ) ) );
  out->setZ( cln.z() );
}

// spherical.p     =
// spherical.theta =
// spherical.pi    =
template< typename REAL >
void 
Coord3Util
::Cyl3ToSph3( const Cylindrical3<REAL>&  cln,
              Spherical3<REAL>*          out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  const REAL  NEAR_ZERO = 1e-5;
  const REAL  z         = cln.z() == static_cast<REAL>(0) ? NEAR_ZERO : cln.z();
  
  out->setTheta( cln.theta() );
  out->setPi( static_cast< REAL >( std::atan( cln.r() / z ) ) );
  out->setRho( static_cast< REAL >( std::sqrt ( cln.r() * cln.r() + z * z ) ) );
}

// vector.x =
// vector.x =
// vector.x =
template< typename REAL >
void 
Coord3Util
::Sph3ToVec3( const Spherical3<REAL>&  sph,
              Vector3<REAL>*           out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );
  
  out->setX( static_cast< REAL >(
    sph.rho() * std::sin( sph.pi() ) * std::sin( sph.theta() ) 
  ) );  
  out->setY( static_cast< REAL >( 
    sph.rho() * std::sin( sph.pi() ) * std::cos( sph.theta() ) 
  ) );
  out->setZ( static_cast< REAL >( sph.rho() * std::cos( sph.pi() ) ) );
}

// cylindrical.r     =
// cylindrical.theta =
// cylindrical.z     =
template< typename REAL >
void 
Coord3Util
::Sph3ToCyl3( const Spherical3<REAL>&  sph,
              Cylindrical3<REAL>*      out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setR( static_cast< REAL >( std::sin( sph.theta() ) * sph.rho() ) );
  out->setTheta( sph.theta() );
  out->setZ( static_cast< REAL >( 
    std::sqrt( sph.rho() * sph.rho() - out->r() * out->r() ) 
  ) );
}

} // namespace Math
} // namespace Gcad

#endif
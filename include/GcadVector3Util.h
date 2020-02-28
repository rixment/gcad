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

#ifndef _GCAD_VECTOR3UTIL_H_
#define _GCAD_VECTOR3UTIL_H_

#include "GcadAssertion.h"
#include "GcadVector3.h"

#include <cmath>

namespace Gcad {
namespace Math {

/** 
  @brief
    Struktura zawierajaca podstawowe operacje, ktore mozna
    wykonywac na trojwymiarowych wektorach
*/
struct Vector3Util {

  /** 
    @brief 
      Wykonanie dzialania dodawania wektorow
    
    @param 
      u Pierwszy argument sumy wektorow
    
    @param 
      v Drugi argument sumy wektorow
    
    @param 
      out Adres wektora, w ktorym umieszczony zostanie wynik operacji
  */
  template< typename REAL >
  static void add( const Vector3<REAL>&  u, 
                   const Vector3<REAL>&  v, 
                   Vector3<REAL>*        out );

  /** 
    @brief 
      Wykonanie arytmetyki odejmowania wektorow
    
    @param 
      u Pierwszy argument roznicy wektorow

    @param 
      v Drugi argument roznicy wektorow

    @param 
      out Adres wektora, w ktorym umieszczony zostanie wynik operacji
  */
  template< typename REAL >
  static void sub( const Vector3<REAL>&  u, 
                   const Vector3<REAL>&  v, 
                   Vector3<REAL>*        out );

  /** 
    @brief 
      Mnozenie wektora przez skalar
    
    @param 
      v Wartosci wektora podlegajace skalowaniu

    @param 
      scalar Wartosc determinujaca czynnik skalowania wektora

    @param 
      out Adres wektora, w ktorym zapisany zostanie wynik operacji
  */
  template< typename REAL >
  static void mul( const Vector3<REAL>&  v, 
                   REAL                  scalar, 
                   Vector3<REAL>*        out );

  /** 
    @brief 
      Dzielenie wektora przez skalar - symetrycznie: mnozenie przez odwrotnosc

    @param 
      v Skladowe wektora podlegajace skalowaniu

    @param 
      scalar Wartosc determinujaca rozmiar skali

    @param 
      out Adres wektora, w ktorym zapisany zostanie wynik operacji

    @remark
      Algebraiczne dzialanie dzielenia skalara przez wektor nie istnieje
  */
  template< typename REAL >
  static void div( const Vector3<REAL>&  v, 
                   REAL                  scalar, 
                   Vector3<REAL>*        out );

  /** 
    @brief 
      Konwersja wektora, na wektor do niego przeciwny

    @param 
      v Wektor poddawany konwersji

    @param 
      out Obszar pamieci dla wynikowego wektora
  */
  template< typename REAL >
  static void invert( const Vector3<REAL>&  v, 
                      Vector3<REAL>*        out );

  /** 
    @brief 
      Dzialanie iloczynu skalarnego wektorow 

    @param 
      u Wektor bioracy udzial w dzialaniu 

    @param 
      v Wektor bioracy udzial w dzialaniu 

    @return 
      Wartosc iloczynu skalarnego wektorow <i>u</i> i <i>v</i>
  */
  template< typename REAL >
  static REAL dot( const Vector3<REAL>&  u, 
                   const Vector3<REAL>&  v );

  /** 
    @brief 
      Dzialanie iloczynu wektorowego wektorow 

    @param 
      u Wektor bioracy udzial w dzialaniu 

    @param 
      v Wektor bioracy udzial w dzialaniu 

    @param 
      out Adres pamieci, w ktorej zostanie zapisany ortogonalny wektor
      wzgledem <i>u</i> i <i>v</i>
  */
  template< typename REAL >
  static void cross( const Vector3<REAL>&  u, 
                     const Vector3<REAL>&  v, 
                     Vector3<REAL>*        out );

  /** 
    @brief 
      Obliczenie dlugosci wektora 
    
    @param 
      v Wartosc okreslajaca wektor, dla ktorego obliczana jest jego dlugosc
  */
  template< typename REAL >
  static REAL length( const Vector3<REAL>& v );

  /** 
    @brief 
      Normalizacja wektora

    @param 
      v Wektor poddawany operacji normalizowania

    @param 
      out Adres pamieci dla znormalizowanej wersji wektora <i>v</i>
  */
  template< typename REAL >
  static void normalize( const Vector3<REAL>&  v, 
                         Vector3<REAL>*        out );

  /**
    @brief
      Wykonanie obliczenia dystansu - wartosci determinujacej dlugosc
      zawarta miedzy zadanymi wektorami
  */
  template< typename REAL >
  static REAL distance( const Vector3<REAL>&  v1,
                        const Vector3<REAL>&  v2 );

  /**
    @brief
      Wykonanie obrotu wektora (vector) wzgledem zadanej osi(axis), oraz
      wartosci kata (angle)

    @param 
      axis Znormalizowany wektor wzgledem ktorego dokonany ma byc obrot
      wektora identyfikowanego przez zmienna vector

    @remark
      3D Math Primer for Graphics and Game Development, page 110
  */
  template< typename REAL >
  static Vector3<REAL> rotateByAxis( const Vector3<REAL>&  axis,
                                     REAL                  angle,
                                     const Vector3<REAL>&  vector );
 
 private:
   // Nie zaimplementowane
   Vector3Util();
   Vector3Util( const Vector3Util& );
   Vector3Util& operator =( const Vector3Util& );

};


//
template< typename REAL >
void 
Vector3Util
::add( const Vector3<REAL>&  u,
       const Vector3<REAL>&  v,
       Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.x() + v.x() );
  out->setY( u.y() + v.y() );
  out->setZ( u.z() + v.z() );
}

//
template< typename REAL >
void 
Vector3Util
::sub( const Vector3<REAL>&  u,
       const Vector3<REAL>&  v,
       Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.x() - v.x() );
  out->setY( u.y() - v.y() );
  out->setZ( u.z() - v.z() );
}

//
template< typename REAL >
void 
Vector3Util
::mul( const Vector3<REAL>&  v,
       REAL                  scalar,
       Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( v.x() * scalar );
  out->setY( v.y() * scalar );
  out->setZ( v.z() * scalar );
}

//
template< typename REAL >
void 
Vector3Util
::div( const Vector3<REAL>&  v,
       REAL                  scalar,
       Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  if( scalar == static_cast<REAL>(0) ) {
    const REAL NEAR_ZERO_VALUE = static_cast<REAL>(1e-5);
    scalar = NEAR_ZERO_VALUE;
  }

  out->setX( v.x() / scalar );
  out->setY( v.y() / scalar );
  out->setZ( v.z() / scalar );
}

//
template< typename REAL >
void 
Vector3Util
::invert( const Vector3<REAL>&  v,
          Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( -v.x() );
  out->setY( -v.y() );
  out->setZ( -v.z() );
}

//
template< typename REAL >
REAL 
Vector3Util
::dot( const Vector3<REAL>&  u,
       const Vector3<REAL>&  v ) 
{
  return static_cast<REAL>( u.x() * v.x() + u.y() * v.y() + u.z() * v.z() );
}

//
template< typename REAL >
void 
Vector3Util
::cross( const Vector3<REAL>&  u,
         const Vector3<REAL>&  v,
         Vector3<REAL>*        out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  out->setX( u.y() * v.z() - u.z() * v.y() );
  out->setY( u.z() * v.x() - u.x() * v.z() );
  out->setZ( u.x() * v.y() - u.y() * v.x() );
}

//
template< typename REAL >
REAL 
Vector3Util
::length( const Vector3<REAL>&  v ) 
{
  const REAL C = dot( v, v );
  return static_cast< REAL >( std::sqrt( C ) );
}

//
template< typename REAL >
void 
Vector3Util
::normalize( const Vector3<REAL>&  v,
                   Vector3<REAL>*  out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );

  div( v, length( v ), out );
}

//
template< typename REAL >
REAL 
Vector3Util
::distance( const Vector3<REAL>&  v1,
            const Vector3<REAL>&  v2 )
{
  Vector3<REAL> vDist;
  sub(v2, v1, &vDist);
  return length(vDist);
}

template< typename REAL >
Vector3<REAL>
Vector3Util
::rotateByAxis( const Vector3<REAL>&  axis,
                REAL                  angle,
                const Vector3<REAL>&  vector )
{
  typedef Vector3<REAL> Vector3;
  
  Vector3 parallelVec(axis * dot(axis, vector));
  Vector3 perpendicularVec(vector - parallelVec);
  Vecotr3 perpendicularToAxis;
  cross(axis, perpendicularVec, &perpendicularToAxis);
  return 
    cos(angle) * perpendicularVec +
    sin(angle) * perpendicularToAxis +
    parallelVec;
}

} // namespace Math
} // namespace Gcad

#endif

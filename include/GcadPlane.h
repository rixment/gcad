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

#ifndef _GCAD_PLANE_H_
#define _GCAD_PLANE_H_

#include "GcadVector3.h"
#include "GcadVector3Util.h"

namespace Gcad {
namespace Math {

//  _     _   __
//  n . ( p - p0 ) = 0
//
//  < nx, ny, nz > . < px - p0x, py - p0y, pz - p0z > = 0
//
//  nxpx + nypy + nzpz - nxp0x - nyp0y - nzp0z = 0
//
//  Ax + By + Cz + D = 0 ( postac ogolna - kanoniczna )
//
//  A = nx; B = ny; C = nz
//  D = - nx * p0x - ny * p0y - nz * p0z

/** 
  @brief
    Klasa reprezentujaca pojecie plaszczyzny, okreslonej
    wektorem normalnym oraz punktem na niej zawartym
*/
template< typename REAL >
class Plane {
 public:  
   /** 
      @brief
        Zbudowanie plaszczyzny, poprzez okreslenie wartosci
        lezacego na niej punktu, oraz jej normalnej
   */
   Plane( const Vector3<REAL>&  p0, 
          const Vector3<REAL>&  n );
   
   /** 
     @brief 
       Determinacja plaszczyzny na podstawie trzech punktow
       na niej zawartych. Parametry okreslaja trzy wspolrzedne 
       tworzace trojkat. Na ich podstawie zostanie zbudowany
       wzor okreslajacy plaszczyzne zawierajaca trojkat uvn
   */
   Plane( const Vector3<REAL>&  u, 
          const Vector3<REAL>&  v, 
          const Vector3<REAL>&  n );

   void setP0( const Vector3<REAL>& p0 );
   void setN( const Vector3<REAL>& n );

   Vector3<REAL> p0() const;
   Vector3<REAL> n() const;

 private:
   Vector3<REAL>  p0_;
   Vector3<REAL>  n_;
};


//
template<typename REAL> 
Plane<REAL>
::Plane( const Vector3<REAL>&  p0, 
         const Vector3<REAL>&  n )
  : p0_( p0 )
  , n_( n )
{
}

//
template<typename REAL> 
Plane<REAL>
::Plane( const Vector3<REAL>&  u,
         const Vector3<REAL>&  v,
         const Vector3<REAL>&  n ) 
  : p0_( u ) 
{
  // Aby wyliczyc wartosci okreslajace plaszczyzne nalezy w pierwszym kroku
  // przypisac wartosc arbitalnie wybrango punktu (w tym wypadku punktu
  // zawartego na plaszczyznie wyznaczonej przez trojkat uvn)
  // do elementu determinujacego punkt zawarty na konstruowanej plaszczyznie.
  // Zadanie to wykonane jest na liscie inicjalizacyjnej konstruktora
  
  // Nastepnie trzeba obliczyc iloczyn wektorowy dwoch wektorow 
  // tworzacych trojkat, w wyniku uzyskujac wartosc wektora normalnego 
  // do powierzchni
  Vector3<REAL>  uTriangle( v - u ),
                 vTriangle( n - u );
  
  Vector3Util::cross( uTriangle, vTriangle, &n_ );
  
  // Powstaly wektor poddac normalizacji otrzymujac w ten sposob 
  // pelny opis matematyczny plaszczyzny (wektor normalny + punkt zawarty
  // na plaszczyznie)
  Vector3Util::normalize( n_, &n_ );  
}

//
template<typename REAL> 
void 
Plane<REAL>
::setP0( const Vector3<REAL>&  p0 ) 
{
  p0_ = p0;
}

//
template<typename REAL> 
void 
Plane<REAL>
::setN( const Vector3<REAL>&  n ) 
{
  n_ = n;
}

//
template<typename REAL> 
Vector3<REAL> 
Plane<REAL>
::p0() const 
{
  return p0_;
}

//
template<typename REAL> 
Vector3<REAL> 
Plane<REAL>
::n() const 
{
  return n_;
}


} // namespace Math
} // namespace Gcad

#endif
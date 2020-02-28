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

#ifndef _GCAD_SPHERICAL3_H_
#define _GCAD_SPHERICAL3_H_

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja wspolrzednych ukladu sferycznego
*/
template< typename REAL >
class Spherical3 {
 public:
   /** 
     @brief 
       Ustalenie wspolrzednych okreslajacych polozenie
       tozsame z punktem (0,0,0) w ukladzie prostokatnym 
   */
   Spherical3();

   /** 
     @brief 
       Nadanie wartosci poczatkowych wspolrzednym
       determinujacym polozenie w przestrzeni 

     @param 
       rho Okresla wartosc rho, czyli odleglosc od srodka
       ukladu, do polozenia punktu

     @param 
       pi Determinuje wartosc kata zawartego pomiedzy
       dodatnia osia z, a prosta wyznaczona przez katy
     
     @param 
       theta Kat zawarty pomiedzy prosta przechodzaca przez
       srodek ukladu, oraz przez rzut punktu na plaszczyzne x-y,
       z dodatnia osia x
   */
   Spherical3( REAL rho, REAL pi, REAL theta );

   void setRho( REAL rho );
   void setPi( REAL pi );
   void setTheta( REAL theta );

   REAL rho()   const;
   REAL pi()    const;
   REAL theta() const;

 private:
   REAL rho_, pi_, theta_;
};


//
template< typename REAL >
Spherical3<REAL>
::Spherical3()
  : rho_( static_cast<REAL>(0) )
  , pi_( static_cast<REAL>(0) )
  , theta_( static_cast<REAL>(0) )
{
}

//
template< typename REAL >
Spherical3<REAL>
::Spherical3( REAL  rho, 
              REAL  pi, 
              REAL  theta )
  : rho_( rho )
  , pi_( pi )
  , theta_( theta )
{
}

//
template< typename REAL >
void
Spherical3<REAL>
::setRho( REAL rho ) 
{
  rho_ = rho;
}

//
template< typename REAL >
void 
Spherical3<REAL>
::setPi( REAL pi ) 
{
  pi_ = pi;
}

//
template< typename REAL >
void 
Spherical3<REAL>
::setTheta( REAL theta ) 
{
  theta_ = theta;
}

//
template< typename REAL >
REAL 
Spherical3<REAL>
::rho() const 
{
  return rho_;
}

//
template< typename REAL >
REAL 
Spherical3<REAL>
::pi() const 
{
  return pi_;
}

//
template< typename REAL >
REAL 
Spherical3<REAL>
::theta() const 
{
  return theta_;
}

} // namespace Math
} // namespace Gcad

#endif

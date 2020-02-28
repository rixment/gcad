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

#ifndef _GCAD_CYLINDRICAL3_H_
#define _GCAD_CYLINDRICAL3_H_

namespace Gcad {
namespace Math {

/** 
  @brief 
    Reprezentacja wspolrzednych cylindrycznych

  @remark 
    Wspolrzedne cylindyczne sa okreslone przez trzy skladowe:
    - r - okreslajaca wartosc rzutu wektora skierowanego, ktorego punkt 
      poczatkowy znajduje sie centrum ukladu, oraz zakonczonym 
      w punkcie (r, theta, z)
    - theta - determinuje kat jaki tworzy rzut wektora 
      na plaszczyzne x-z z osia x
    - z - czynnik tozsamy z wspolrzedna z ukladu prostokatnego
*/
template< typename REAL >
class Cylindrical3 {
 public:
   Cylindrical3();
   Cylindrical3( REAL r, REAL theta, REAL z );

   void setR( REAL r );
   void setTheta( REAL theta );
   void setZ( REAL z );

   REAL r() const;
   REAL theta() const;
   REAL z() const;

 private:
   REAL r_, theta_, z_;
};


//
template< typename REAL >
Cylindrical3<REAL>
::Cylindrical3()
  : r_( static_cast<REAL>(0) )
  , theta_( static_cast<REAL>(0) )
  , z_( static_cast<REAL>(0) )
{
}

//
template< typename REAL >
Cylindrical3<REAL>
::Cylindrical3( REAL r, REAL theta, REAL z )
  : r_( r )
  , theta_( theta )
  , z_( z )
{
}

//
template< typename REAL >
void 
Cylindrical3<REAL>
::setR( REAL r ) 
{
  r_ = r;
}

//
template< typename REAL >
void 
Cylindrical3<REAL>
::setTheta( REAL theta ) 
{
  theta_ = theta;
}

//
template< typename REAL >
void 
Cylindrical3<REAL>
::setZ( REAL z ) 
{
  z_ = z;
}

//
template< typename REAL >
REAL 
Cylindrical3<REAL>
::r() const 
{
  return r_;
}
  
//
template< typename REAL >
REAL
Cylindrical3<REAL>
::theta() const 
{
  return theta_;
}
 
//
template< typename REAL >
REAL 
Cylindrical3<REAL>
::z() const 
{
  return z_;
}

} // namespace Math
} // namespace Gcad

#endif
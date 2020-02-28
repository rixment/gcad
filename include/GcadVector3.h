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

#ifndef _GCAD_VECTOR3_H_
#define _GCAD_VECTOR3_H_

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja obiektu matematycznego - trojwymiarowego wektora

  @param
    REAL Parametr szablonu okreslajacy nazwe typu determinujacego
    wartosci wspolrzednych wektora. Podczas konkretyzacji szablonu
    powinny byc wykorzystywane typy fundamentalne rodzaju <i>float</i>,
    lub <i>double</i>
*/
template< typename REAL >
class Vector3 {
 public:
   Vector3();
   Vector3( REAL x, REAL y, REAL z );

   void setX( REAL x );
   void setY( REAL y );
   void setZ( REAL z );

   Vector3& operator +=( const Vector3& vector );
   Vector3& operator -=( const Vector3& vector );
   Vector3& operator *=( REAL scalar );
   Vector3& operator /=( REAL scalar );

   REAL x() const;
   REAL y() const;
   REAL z() const;

 private:
   REAL  x_, y_, z_;
};


//
template< typename REAL >
Vector3<REAL>
::Vector3()
  : x_( static_cast<REAL>(0) )
  , y_( static_cast<REAL>(0) )
  , z_( static_cast<REAL>(0) )
{
}

//
template< typename REAL >
Vector3<REAL>
::Vector3( REAL  x, 
           REAL  y, 
           REAL  z )
  : x_( x )
  , y_( y )
  , z_( z )
{
}

//
template< typename REAL >
void 
Vector3<REAL>
::setX( REAL x ) 
{
  x_ = x;
}
 
//
template< typename REAL >
void 
Vector3<REAL>
::setY( REAL y ) 
{
  y_ = y;
}
  
//
template< typename REAL >
void 
Vector3<REAL>
::setZ( REAL z ) 
{
  z_ = z;
}

//
template< typename REAL >
Vector3<REAL>& 
Vector3<REAL>
::operator +=( const Vector3& vector ) 
{
  x_ += vector.x();
  y_ += vector.y();
  z_ += vector.z();
  return *this;
}

//
template< typename REAL >
Vector3<REAL>& 
Vector3<REAL>
::operator -=( const Vector3& vector ) 
{
  x_ -= vector.x();
  y_ -= vector.y();
  z_ -= vector.z();
  return *this;
}

//
template< typename REAL >
Vector3<REAL>& 
Vector3<REAL>
::operator *=( REAL scalar ) 
{
  x_ *= scalar;
  y_ *= scalar;
  z_ *= scalar;
  return *this;
}

//
template< typename REAL >
Vector3<REAL>& 
Vector3<REAL>
::operator /=( REAL scalar ) 
{
  if( scalar == static_cast<REAL>(0) ) {
    const REAL NEAR_ZERO_VALUE = static_cast<REAL>(1e-5);
    scalar = NEAR_ZERO_VALUE;
  }

  x_ /= scalar;
  y_ /= scalar;
  z_ /= scalar;
  return *this;
}

//
template< typename REAL >
REAL 
Vector3<REAL>
::x() const 
{
  return x_;
}

//
template< typename REAL >
REAL
Vector3<REAL>
::y() const 
{
  return y_;
}

//
template< typename REAL >
REAL 
Vector3<REAL>
::z() const 
{
  return z_;
}

//
template< typename REAL >
bool
operator ==( const Vector3<REAL>&  u,
             const Vector3<REAL>&  v ) 
{
  return u.x() == v.x() && 
    u.y() == v.y() &&
    u.z() == v.z();
}

//
template< typename REAL >
bool
operator !=( const Vector3<REAL>&  u,
             const Vector3<REAL>&  v ) 
{
  return !( u == v );
}

//
template< typename REAL >
const Vector3<REAL>
operator +( const Vector3<REAL>&  u,
            const Vector3<REAL>&  v ) 
{
  return Vector3<REAL>( u ) += v;
}

//
template< typename REAL >
const Vector3<REAL>
operator -( const Vector3<REAL>&  u,
            const Vector3<REAL>&  v ) 
{
  return Vector3<REAL>( u ) -= v;
}

//
template< typename REAL >
const Vector3<REAL>
operator *( const Vector3<REAL>&  u,
            REAL                  scalar ) 
{
  return Vector3<REAL>( u ) *= scalar;
}

//
template< typename REAL >
const Vector3<REAL>
operator *( REAL                  scalar,
            const Vector3<REAL>&  u ) 
{
  return u * scalar;
}

//
template< typename REAL >
const Vector3<REAL>
operator /( const Vector3<REAL>&  u,
            REAL                  scalar ) 
{
  return Vector3<REAL>( u ) /= scalar;
}

//
template< typename REAL >
const Vector3<REAL>
operator -( const Vector3<REAL>& u ) 
{
  return Vector3<REAL>( -u.x(), -u.y(), -u.z() );
}

} // namespace Math
} // namespace Gcad

#endif

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

#ifndef _GCAD_VECTOR2_H_
#define _GCAD_VECTOR2_H_

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja wektora dwu-wymiarowego
*/
template< typename REAL >
class Vector2 {
 public:
   Vector2();
   Vector2( REAL x, REAL y );

   void setX( REAL x );
   void setY( REAL y );
     
   REAL x() const;
   REAL y() const;

   Vector2& operator +=( const Vector2& vector );
   Vector2& operator -=( const Vector2& vector );
   Vector2& operator *=( REAL scalar );
   Vector2& operator /=( REAL scalar );

 private:
   REAL  x_;
   REAL  y_;
};


//
template<typename REAL> 
Vector2<REAL>
::Vector2()
  : x_( static_cast<REAL>(0) )
  , y_( static_cast<REAL>(0) )
{
}

//
template<typename REAL> 
Vector2<REAL>
::Vector2( REAL  x, 
           REAL  y )
  : x_( x )
  , y_( y )
{
}

//
template<typename REAL> 
void 
Vector2<REAL>
::setX( REAL x ) 
{
  x_ = x;
}

//
template<typename REAL>
void 
Vector2<REAL>
::setY( REAL y ) 
{
  y_ = y;
}
  
//
template<typename REAL> 
REAL 
Vector2<REAL>
::x() const 
{
  return x_;
}

//
template<typename REAL> 
REAL 
Vector2<REAL>
::y() const 
{
  return y_;
}

//
template<typename REAL>
bool
operator ==( const Vector2<REAL>&  u,
             const Vector2<REAL>&  v ) 
{
  return u.x() == v.x() && 
    u.y() == v.y();
}

//
template<typename REAL> 
bool
operator !=( const Vector2<REAL>&  u,
             const Vector2<REAL>&  v ) 
{
  return !( u == v );
}

//
template<typename REAL> 
Vector2<REAL>& 
Vector2<REAL>
::operator +=( const Vector2& vector ) 
{
  x_ += vector.x();
  y_ += vector.y();
  return *this;
}

//
template<typename REAL> 
Vector2<REAL>& 
Vector2<REAL>
::operator -=( const Vector2& vector ) 
{
  x_ -= vector.x();
  y_ -= vector.y();
  return *this;
}

//
template<typename REAL> 
Vector2<REAL>& 
Vector2<REAL>
::operator *=( REAL scalar ) 
{
  x_ *= scalar;
  y_ *= scalar;
  return *this;
}

//
template<typename REAL> 
Vector2<REAL>& 
Vector2<REAL>
::operator /=( REAL scalar ) 
{
  if( scalar == static_cast<REAL>(0) ) {
    const REAL NEAR_ZERO = 1e-5;
    scalar = NEAR_ZERO;
  }

  x_ /= scalar;
  y_ /= scalar;
  return *this;
}

//
template<typename REAL> 
Vector2<REAL>
operator +( const Vector2<REAL>&  u,
            const Vector2<REAL>&  v ) 
{
  return Vector2<REAL>( u.x(), u.y() ) += v;
}

//
template<typename REAL> 
Vector2<REAL>
operator -( const Vector2<REAL>&  u,
            const Vector2<REAL>&  v )
{
  return Vector2<REAL>( u.x(), u.y() ) -= v;
}

//
template<typename REAL> 
Vector2<REAL>
operator *( const Vector2<REAL>&  u,
            REAL                  scalar ) 
{
  return Vector2<REAL>( u.x(), u.y() ) *= scalar;
}

//
template<typename REAL>
Vector2<REAL>
operator *( REAL                  scalar,
            const Vector2<REAL>&  u ) 
{
  return u * scalar;
}

//
template<typename REAL> 
Vector2<REAL>
operator /( const Vector2<REAL>&  u,
            REAL                  scalar ) 
{
  return Vector2<REAL>( u.x(), u.y() ) /= scalar;
}

//
template<typename REAL> 
Vector2<REAL>
operator -( const Vector2<REAL>& u ) 
{
  return Vector2<REAL>( -u.x(), -u.y() );
}

} // namespace Math
} // namespace Gcad

#endif

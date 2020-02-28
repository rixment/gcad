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

#ifndef _GCAD_PARAMLINE2_H_
#define _GCAD_PARAMLINE2_H_

#include "GcadVector2.h"

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja pojecia prostej parametrycznej 
    w przestrzeni dwu-wymiarowej
*/
template< typename REAL >
class ParamLine2 {
 public:   
   ParamLine2();

   /** 
     @brief 
       Na podstawie przekazanych wartosci, 
       zostaje skonstruowana prosta parametryczna

     @param 
       p0 Punkt poczatkowy prostej

     @param 
       v Wektor kierunkowy

     @param 
       t Czynnik determinujacy odcinek, zawarty na prostej
   */
   ParamLine2( const Vector2<REAL>&  p0, 
               const Vector2<REAL>&  v, 
               REAL                  t = static_cast<REAL>(1) );

   void setP0( const Vector2<REAL>&  p0 );
   void setV( const Vector2<REAL>&   v );
   void setT( REAL t );

   Vector2<REAL> p0() const;
   Vector2<REAL> v() const;
   REAL t() const;

 private:          
   Vector2<REAL>  p0_; /**< Punkt poczatkowy parametrycznej prostej */
   Vector2<REAL>  v_;  /**< Wektor kierunkowy */
   REAL           t_;  /**< Wspolczynnik skalowania wektora kierunkowego */
};


template<typename REAL>
ParamLine2<REAL>
::ParamLine2() 
  : t_( static_cast<REAL>(0) )
{
}

template<typename REAL> 
ParamLine2<REAL>
::ParamLine2( const Vector2<REAL>&  p0,
              const Vector2<REAL>&  v,
              REAL                  t )
  : p0_( p0 )
  , v_( v )
  , t_( t ) 
{
}

template<typename REAL> 
void 
ParamLine2<REAL>
::setP0( const Vector2<REAL>&  p0 ) 
{
  p0_ = p0;
}

template<typename REAL>
void 
ParamLine2<REAL>
::setV( const Vector2<REAL>&  v ) 
{
  v_ = v;
}

template<typename REAL> 
void 
ParamLine2<REAL>
::setT( REAL t ) 
{
  t_ = t;
}

template<typename REAL>
Vector2<REAL> 
ParamLine2<REAL>
::p0() const 
{
  return p0_;
}

template<typename REAL>
Vector2<REAL> 
ParamLine2<REAL>
::v() const
{
  return v_;
}

template<typename REAL> REAL 
ParamLine2<REAL>
::t() const 
{
  return t_;
}

template<typename REAL> 
bool
operator ==( const ParamLine2<REAL>&  k,
             const ParamLine2<REAL>&  l ) 
{
  return k.p0() == l.p0() && 
    k.v() == l.v() && 
    k.t() == l.t();
}

template<typename REAL> 
bool
operator !=( const ParamLine2<REAL>&  k,
             const ParamLine2<REAL>&  l ) 
{
  return !( k == l );
}
                     
} // namespace Math
} // namespace Gcad

#endif
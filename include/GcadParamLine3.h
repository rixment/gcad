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

#ifndef _GCAD_PARAMLINE3_H_
#define _GCAD_PARAMLINE3_H_

#include "GcadVector3.h"

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja prostej parametrycznej w przestrzeni
    trojwymiarowej
*/
template< typename REAL >
class ParamLine3 {
 public:
   ParamLine3();
   
   /** 
     @brief 
       Konstruktor determinujacy odcinek
     
     @param 
       p0 Punkt poczatkowy odcinka
     
     @param 
       v Wektor kierunkowy
     
     @param 
       t Wspolczynnik skali odcinka
   */       
   ParamLine3( const Vector3<REAL>&  p0, 
               const Vector3<REAL>&  v, 
               REAL                  t );
   
   /** 
     @brief 
       Okreslenie prostej (odcinka) na podstawie
       przekazanych argumentow determinujacych poczatek
       oraz koniec odcinka w przestrzeni trojwymiarowej
   */
   ParamLine3( const Vector3<REAL>&  p0, 
               const Vector3<REAL>&  p1 );

   void setP0( const Vector3<REAL>&  p0 );
   void setV( const Vector3<REAL>&   v );
   void setT( REAL t );

   Vector3<REAL> p0() const;
   Vector3<REAL> v()  const;
   REAL          t()  const;

 private: 
   Vector3<REAL>  p0_; /**< Punkt poczatkowy prostej */
   Vector3<REAL>  v_;  /**< Wektor kierunkowy */
   REAL           t_;  /**< Wspolczynnik skalowania odcinka */
};


template< typename REAL >
ParamLine3<REAL>
::ParamLine3()
  : t_( static_cast<REAL>(0) )
{
}

template< typename REAL >
ParamLine3<REAL>
::ParamLine3( const Vector3<REAL>&  p0,
              const Vector3<REAL>&  v,
              REAL                  t )
  : p0_( p0 )
  , v_( v )
  , t_( t ) 
{
}

template< typename REAL >
ParamLine3<REAL>
::ParamLine3( const Vector3<REAL>& p0,
              const Vector3<REAL>& p1 )
  : p0_( p0 )
  , v_( p1 - p0 )
  , t_( static_cast<REAL>(1) ) 
{
}

template< typename REAL >
void 
ParamLine3<REAL>
::setP0( const Vector3<REAL>&  p0 ) 
{
  p0_ = p0;
}

template< typename REAL > 
void 
ParamLine3<REAL>
::setV( const Vector3<REAL>&  v ) 
{
  v_ = v;
}

template< typename REAL > 
void 
ParamLine3<REAL>
::setT( REAL  t ) 
{
  t_ = t;
}

template< typename REAL > 
Vector3<REAL> 
ParamLine3<REAL>
::p0() const 
{
  return p0_;
}

template< typename REAL > 
Vector3<REAL> 
ParamLine3<REAL>
::v() const 
{
  return v_;
}

template< typename REAL > 
REAL 
ParamLine3<REAL>
::t() const 
{
  return t_;
}

template< typename REAL > 
bool
operator ==( const ParamLine3<REAL>&  l,
             const ParamLine3<REAL>&  k ) 
{
  return l.p0() == k.p0() && 
    l.v() == k.v() && 
    l.t() == k.t();
}

template< typename REAL > 
bool
operator !=( const ParamLine3<REAL>&  l,
             const ParamLine3<REAL>&  k ) 
{
  return !( l == k );
}

} // namespace Math
} // namespace Gcad

#endif
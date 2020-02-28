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

#ifndef _GCAD_POLAR2_H_
#define _GCAD_POLAR2_H_

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja wspolrzednych polarnych - uklad dwuwymiarowy
*/
template<typename REAL>
class Polar2 {
 public:
   Polar2();
   Polar2( REAL r, REAL theta );

   void setR( REAL r );
   void setTheta( REAL theta );

   REAL r() const;
   REAL theta() const;

 private:
   REAL r_;
   REAL theta_;
};


//
template<typename REAL> 
Polar2<REAL>
::Polar2()
  : r_( static_cast<REAL>(0) )
  , theta_( static_cast<REAL>(0) )
{
}
  
//
template<typename REAL> 
Polar2<REAL>
::Polar2( REAL r, 
          REAL theta )
  : r_( r )
  , theta_( theta )
{
}

//
template<typename REAL> 
void 
Polar2<REAL>
::setR( REAL r ) 
{
  r_ = r;
}
 
//
template<typename REAL> 
void 
Polar2<REAL>
::setTheta( REAL theta ) 
{
  theta_ = theta;
}

//
template<typename REAL>
REAL 
Polar2<REAL>
::r() const 
{
  return r_;
}

//
template<typename REAL> 
REAL 
Polar2<REAL>
::theta() const 
{
  return theta_;
}

//
template<typename REAL> 
bool
operator ==( const Polar2<REAL>&  p,
             const Polar2<REAL>&  q ) 
{
  return p.r() == q.r() && 
    p.theta() == q.theta();
}

//
template<typename REAL>
bool
operator !=( const Polar2<REAL>&  p,
             const Polar2<REAL>&  q ) 
{
  return !( p == q );
}

} // namespace Math
} // namespace Gcad

#endif

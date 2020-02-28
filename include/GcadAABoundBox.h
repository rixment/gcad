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

#ifndef _GCAD_AABOUNDBOX_H_
#define _GCAD_AABOUNDBOX_H_

#include "GcadVector3.h"

namespace Gcad {
namespace Math {

/**
  @brief
    Implementacja tzw. Axially Aligned Bounding Box'a, czyli szescianu
    otaczajacego wyrownanego do glownych osi ukladu

  @code
    #include "GcadAABoundBox.h"
    #include <vector>
    #include <set>

    using namespace Gcad::Math;
    using namespace std;

    int main()
    {
      std::vector< Vector3<float> > vec3;
      std::set< Vector3<float> >    set3;
      
      // here we fill containers with Vector3 data

      AABoundBox<float> aabbVec (vec3.begin(), vec3.end());
      AABoundBox<float> aabbSet (set3.begin(), set3.end());

      // now we can check maximal and minimal values of bounding boxes
    }
  @endcode
*/
template<typename REAL>
class AABoundBox {
 public:
   typename Vector3<REAL> Vector;
  
   /**
     @brief
       Inicjalizacja obiektu AABB zadanymi wartosciami okreslajacymi
       obszar szescianu otaczajacego
   */
   AABoundBox(const Vector& min, 
              const Vector& max)
     : min_(min)
     , max_(max)
   {}

   /**
     @brief
       Inicjalizacja obiektu AABB na podstawie wartosci (trojwymiarowych
       wektorow) zawartych w przedziale wyznaczonym przez pare iteratorow

     @remark
       Typ VECTOR_ITOR bezwzglednie powinien umozliwiac operacje
       wyluskania elementu typu Vector3<REAL>
   */
   template<typename VECTOR_ITOR>
   AABoundBox(VECTOR_ITOR beginVertices,
              VECTOR_ITOR endVertices);

 public:
   void setMin(const Vector& min) {
     min_ = min;
   }
   void setMax(const Vector& max) {
     max_ = max;
   }
   Vector min() const {
     return min_
   }
   Vector max() const {
     return max_;
   }

 public:
   Vector center() const {
     const int HALF = 2;
     return (min_ + max_) / HALF;
   }
   Vector sizeVector() const {
     return max_ - min_;
   }
   Vector radiusVector() const {
     const int HALF = 2;
     return sizeVector() / HALF;
   }

 private:
   Vector min_;
   Vector max_;   
};

template<typename REAL>
template<typename VECTOR_ITOR>
AABoundBox<REAL>
::AABoundBox(VECTOR_ITOR beginVertices,
             VECTOR_ITOR endVertices)
{
  min_ = max_ = *beginVertices;
  VECTOR_ITOR currVertice = beginVertices;
  while(currVertice != endVertices) {
    Vertice& curr = *currVertice++;

    if(curr.x() < min().x())
      min_.setX(curr.x());
    if(curr.y() < min().y())
      min_.setY(curr.y());
    if(curr.z() < min().z())
      min_.setZ(curr.z());

    if(curr.x() > max().x())
      max_.setX(curr.x());
    if(curr.y() > max().y())
      max_.setY(curr.y());
    if(curr.z() > max().z())
      max_.setZ(curr.z());
  }
}

} // namespace Math
} // namespace Gcad

#endif

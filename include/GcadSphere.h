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

#ifndef _GCAD_SPHERE_H_
#define _GCAD_SPHERE_H_

#include "GcadVector3.h"
#include "GcadVector3Utils.h"

namespace Gcad {
namespace Math {

template<typename REAL>
class Sphere {
 public:
   explicit Sphere(REAL radius)
     : radius_(radius)
   {}
   Shpere(REAL radius, const Vector3<REAL>& origin)
     : radius_(radius)
     , origin_(origin)
   {}
 
 public:
   void setRadius(REAL radius) {
     radius_ = radius;
   }
   void setOrigin(const Vector3<REAL>& origin) {
     origin_ = origin;
   }
   REAL radius() const {
     return radius_;
   }
   Vector3<REAL> origin() const {
     return origin_;
   }

 public:
   REAL volume() const { 
     return 4/3 * PI * radius_ * radius_ * radius_; 
   }
   REAL surfaceArea() const {
     return 4 * PI * radius_ * radius_;
   }

 public:
   bool intersect(const Sphere<REAL>& sphere) const {
     return Vector3Util::length(orign() - sphere.orign()) >=
       Vector3Util::distance(orign(), sphere.orign()) 
   }

 private:
   Vector3<REAL> orign_;
   REAL radius_;
};

} // namespace Math
} // namespace Gcad

#endif

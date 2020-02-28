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

#ifndef _GCAD_REFARRAYCNTPTR_H_
#define _GCAD_REFARRAYCNTPTR_H_

#include "GcadRefCountPtr.h"

namespace Gcad {
namespace Utilities {

template<typename T>
class RefArrayCntPtr : public RefCountPtr<T> {
 public:
   RefArrayCntPtr( T* pointer = 0 )
     : RefCountPtr<T>(pointer)
   {}
   
   RefArrayCntPtr( const RefArrayCntPtr& that )
     : RefCountPtr<T>(that)
   {}

   T& operator [](size_t index) const {
     return *( get() + index );
   }

   ~RefArrayCntPtr() {
     T* array = release();
     delete [] array;
   }
};

} // namespace Utilities
} // namespace Gcad

#endif

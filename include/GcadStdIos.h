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

#ifndef _GCAD_STDIOS_H_
#define _GCAD_STDIOS_H_

#include <iostream>

namespace Gcad {
namespace Utilities {

template<typename T>
void
readFromStdStream(std::istream&   source, 
                  T*              destination,
                  std::streamsize elementsCount)
{
  source.read( 
    reinterpret_cast<char*>(destination), 
    sizeof(T) * elementsCount);
}

template<typename T>
void
writeIntoStdStream(std::ostream&   destination, 
                   const T&        source, 
                   std::streamsize elementsCount)
{
  destination.write( 
    reinterpret_cast<const char*>(&source), 
    sizeof(T) * elementsCount);
}

} // namespace Utilities
} // namespace Gcad

#endif

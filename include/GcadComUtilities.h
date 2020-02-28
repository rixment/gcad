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

#ifndef _GCAD_COMUTILITIES_H_
#define _GCAD_COMUTILITIES_H_

#include "GcadAssertion.h"

namespace Gcad {
namespace Utilities {

/**
  @brief
    Funkcja przypisujaca zmiennej przekazanej w formie drugiego argumentu
    wywolania interfejs COM pierwszego parametru

  @param
    lhsComInterface Adres poprawnego interfejsu, ktory jest zrodlem 
    operacji przypisania

  @param
    rhsComInterface Adres wskaznika interfejsu, ktory posluzy jako
    docelowy obiekt przypisania
*/
template< typename COM_INTERFACE >
void 
assignCOMInterface( COM_INTERFACE*   lhsComInterface,
                    COM_INTERFACE**  rhsComInterface )
{
  assertion( lhsComInterface != 0,
    "Brak wskazanego pozadanego interfejsu COM!" );

  assertion( rhsComInterface != 0 && *rhsComInterface != 0,
    "Bledny docelowy interfejs COM!" );

  *rhsComInterface = lhsComInterface;
  lhsComInterface->AddRef();
}

/**
  @brief
    Funkcja zwalniajaca w bezpieczny sposob przekazany w formie 
    argumentu interfejs programowy technologi COM

  @param
    comInterface Adres wskaznika interfejsu, ktory zostaje poddany
    operacji zwolnienia
*/
template< typename COM_INTERFACE >
void
releaseCOMInterface( COM_INTERFACE** comInterface )
{
  assertion( comInterface != 0 && *comInterface != 0,
    "Przekazano bledny interfejs COM!" );

  (*comInterface)->Release();
  *comInterface = 0;
}

} // namespace Utilities
} // namespace Gcad

#endif
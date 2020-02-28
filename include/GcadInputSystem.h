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

#ifndef _GCAD_INPUTSYSTEM_H_
#define _GCAD_INPUTSYSTEM_H_

#include "GcadBase.h"
#include "GcadKeyboard.h"
#include "GcadMouse.h"

namespace Gcad {
namespace Platform {

/** 
  @brief
    Glowny obiekt podsystemu wejscia
*/
class GCAD_EXPORT InputSystem {
 public:
   virtual ~InputSystem();

   //! @brief Przeprowadzenie wstepnej inicjalizacji podsystemu wejscia
   virtual bool initialize() = 0;
   
   //! @brief Zakonczenie pracy podsystemu wejscia
   virtual void shutDown() = 0;
   
   //! @brief Aktualizacja wewnetrznych struktur danych urzadzen wejscia
   virtual void update() = 0;
  
   //! @brief Udostepnienie interfejsu klawiatury
   virtual Keyboard& keyboard() = 0;
   
   //! @brief Udostepnienie interfejsu myszy
   virtual Mouse& mouse() = 0;
};

} // namespace Platform
} // namespace Gcad

#endif

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

#ifndef _GCAD_WINDOW_H_
#define _GCAD_WINDOW_H_

#include "GcadException.h"
#include <windows.h>
#include <string>

namespace Gcad {
namespace Platform {

class Window {
 public:
   class CreationError 
     : public Gcad::Utilities::Exception
   {
     CreationError(const std::string& e)
       : Gcad::Utilities::Exception(e)
     {}
   };

   Window(int width, int height, const std::string& caption);

   int getWidth() const { return width_; }
   int getHeight() const { return height_; }
   std::string getCatpion() const { return caption_; }
   HWND getHWND() const { return hwnd_; }

 private:
   int width_;
   int height_;
   std::string caption_;
   HWND hwnd_;
};

} // namespace Platform
} // namespace Gcad

#endif

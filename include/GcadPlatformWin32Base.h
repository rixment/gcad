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

#ifndef _GCAD_PLATFORMWIN32BASE_H_
#define _GCAD_PLATFORMWIN32BASE_H_

#ifdef _MSC_VER
  #pragma warning (disable : 4251)
#endif

#ifdef WIN32
  #ifdef GCAD_DLL_LIBRARY
    #define GCAD_WIN32_EXPORT __declspec(dllexport)
  #elif defined (GCAD_LIB)
    #define GCAD_WIN32_EXPORT
  #else
    #define GCAD_WIN32_EXPORT __declspec(dllimport)
  #endif
#else
  #define GCAD_WIN32_EXPORT
#endif

#endif
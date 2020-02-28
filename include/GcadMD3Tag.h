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

#ifndef _GCAD_MD3TAG_H_
#define _GCAD_MD3TAG_H_

#include "GcadMatrix.h"
#include "GcadQuaternion.h"
#include "GcadVector3.h"
#include <iostream>
#include <string>

namespace Gcad {
namespace Framework {

/**
  @brief 
    Opis lacznika elementu siatki modelu MD3
*/
class MD3Tag {
 public:
   /** 
     @brief 
       Synonim macierzy rzedu 3 dla opisu obrotu 
       wyszczegolnionego komponentu modelu
   */
   typedef Gcad::Math::Matrix<3, 3, float>   Matrix3x3;
   
   typedef Gcad::Math::Vector3<float>    Vector3;
   typedef Gcad::Math::Quaternion<float> Quaternion;

   /**
     @brief
       Konstruktor wczytujacy dane strumienia wejsciowego
   */
   MD3Tag( std::istream& tagDataStream );

   /**
     @brief
       Opisowa nazwa lacznika
   */
   std::string name() const;
   
   /**
     @brief
       Wartosc okreslajaca pozycje lacznika
   */
   Vector3 position() const;
   
   /**
     @brief
       Orientacja komponentu modelu sprzezonego z lacznikiem
   */
   Matrix3x3 orientation() const;

   /**
     @brief
       Orientacja komponentu wyrazona w postaci kwaternionu
   */
   Quaternion quatOrient() const;

 private:
   char       name_[ 64 ];  /**< @brief Opis lacznika */
   Vector3    position_;    /**< @brief Pozycja lacznika */
   Matrix3x3  orientation_; /**< @brief Orientacja czesci siatki */
   Quaternion quatOrient_;  /**< @brief Orientacja w postaci kwaternionowej */
};


//
inline std::string MD3Tag
::name() const
{
  return name_;
}
   
//
inline MD3Tag::Vector3 MD3Tag
::position() const
{
  return position_;
}
   
//
inline MD3Tag::Matrix3x3 MD3Tag
::orientation() const
{
  return orientation_;
}

inline MD3Tag::Quaternion MD3Tag
::quatOrient() const
{
  return quatOrient_;
}

} // namespace Framework
} // namespace Gcad

#endif

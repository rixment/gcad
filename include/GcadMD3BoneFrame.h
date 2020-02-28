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

#ifndef _GCAD_MD3BONEFRAME_H_
#define _GCAD_MD3BONEFRAME_H_

#include "GcadVector3.h"
#include <iostream>
#include <string>

namespace Gcad {
namespace Framework {

/**
  @brief
    Informacje dotyczace pojedynczego elementu skladowego modelu,
    odpowiadajace wyszczegolnionej klatce kluczowej

  @remark
    Wspolrzedne minimalne i maksymalne szescianu otaczajacego sa
    zdefiniowane wzgledem wspolrzednej srodka modelu (origin_)
*/
class MD3BoneFrame {
   typedef Gcad::Math::Vector3<float> Vector3;
 public: 
  /**
     @brief
       Konstruktor wykonujacy prace wczytania danych wejsciowych,
       oraz zapewniajacy odpowiedni format danych skladowych
   */
   MD3BoneFrame( std::istream& boneFrameData );

   /**
     @brief
       Akcesor do wartosci determinujacej pierwszy 
       z rogow szescianu otaczajacego
   */
   Vector3 minBoundingBox() const;

   /**
     @brief
       Akcesor do wartosci determinujacej drugi
       z rogow szescianu otaczajacego
   */
   Vector3 maxBoundingBox() const;

   /**
     @brief
       Srodek siatki geometrycznej modelu
   */
   Vector3 origin() const;

   /**
     @brief
       Promien sfery otaczajacej
   */
   float sphereRadius() const;
     
   /**
     @brief
       Nazwa przetwarzanej klatki
   */
   std::string name() const;

 private:
   Vector3  minBBox_;      /**< @brief Pierwszy rog otaczajacego szescianu */
   Vector3  maxBBox_;      /**< @brief Drugi rog otaczajacego szescianu */
   Vector3  originPos_;    /**< @brief Srodek szescianu/sfery otaczajacej */
   float    sphereRadius_; /**< @brief Promien sfery otaczajacej */
   char     name_[ 16 ];   /**< @brief Nazwa klatki, zakonczonej wartoscia NULL */
};


//
inline Gcad::Math::Vector3<float> MD3BoneFrame
::minBoundingBox() const
{
  return minBBox_;
}

//
inline Gcad::Math::Vector3<float> MD3BoneFrame
::maxBoundingBox() const
{
  return maxBBox_;
}

//
inline Gcad::Math::Vector3<float> MD3BoneFrame
::origin() const
{
  return originPos_;
}

//
inline float MD3BoneFrame
::sphereRadius() const
{
  return sphereRadius_;
}
     
//
inline std::string MD3BoneFrame
::name() const
{
  return name_;
}

} // namespace Framework
} // namespace Gcad

#endif

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

#ifndef _GCAD_MD2ANIMATOR_H_
#define _GCAD_MD2ANIMATOR_H_

#include "GcadBase.h"
#include "GcadVector3.h"
#include <vector>
#include <memory>

namespace Gcad {
namespace Framework {

class GCAD_EXPORT MD2Data;

/**
  @brief
    Klasa instancji umozliwiajacych obliczenia klatek
    posrednich sekwencji kluczowych animacji formatu MD2
*/
class GCAD_EXPORT MD2Animator {
 public:
   enum AnimType {
     AT_SINGLE, /**< Jednorazowe odtworzenia sekwencji animacyjnej */
     AT_LOOP    /**< Odtwarzanie animacji w petli */
   };

   /**
     @brief Dostepne sekwencje animacyjne modelu MD2
   */
   enum AnimSequence {
     AS_IDLE,
     AS_RUN,
     AS_ATTACK,
     AS_PAIN1,
     AS_PAIN2,
     AS_PAIN3,
     AS_JUMP,
     AS_FLIP,
     AS_SALUTE,
     AS_TAUNT,
     AS_WAVE,
     AS_POINT,
     AS_CROUCH_STAND,
     AS_CROUCH_WALK,
     AS_CROUCH_ATTACK,
     AS_CROUCH_PAIN,
     AS_CROUCH_DEATCH,
     AS_DEATH_BACK,
     AS_DEATH_FOWARD,
     AS_DEATH_SLOW
   };

   typedef Gcad::Math::Vector3<float>  Vector3;

   typedef std::vector< Vector3 >             VerticesKeyFrame;
   typedef VerticesKeyFrame::iterator         VerticesKeyFrameItor;
   typedef std::auto_ptr< VerticesKeyFrame >  VerticesKeyFrameAutoPtr;

   typedef std::vector< Vector3 >            NormalsKeyFrame;
   typedef NormalsKeyFrame::iterator         NormalsKeyFrameItor;
   typedef std::auto_ptr< NormalsKeyFrame >  NormalsKeyFrameAutoPtr;

   /**
     @brief Konstruktor wiazacy iterator z agregatem danych siatki
   */
   MD2Animator( const MD2Data& parentMeshData );

   /**
     @brief
       Udostepnienie interpolowanej klatki animacji - danych wierzcholkow
   */
   VerticesKeyFrameAutoPtr interpolatedVertices() const;

   /**
     @brief 
       Udostepnienie interpolowanej klatki animacji - danych normalnych
   */
   NormalsKeyFrameAutoPtr interpolatedNormals() const;

   /**
     @brief 
       Okreslenie biezacej animacji, oraz sposobu jej odegrania
   */
   void setAnimation( AnimSequence  animationSequence,
                      AnimType      animationType );

   /**
     @brief 
       Ustalenie wzglednej szybkosci odtwarzania animacji
   */
   void setSpeedFactor( float speedFactor );

   /**
     @brief Metoda odpowiedzialna za aktualizacje animacji
   */
   void update( float elapsedTime );

   /**
     @brief Informacja dotyczaca rodzica obiektu animatora
   */
   const MD2Data& parentMesh() const;

 private:
   const MD2Data&  meshData_;
 
   AnimSequence    currentAnimSeq_;  /**< Aktualnie odtwarzana sekwencja */
   AnimType        currentAnimType_; /**< Rodzaj odtwarzania animacji */

   int             firstInterpolatedFrame_;
   int             secondInterpolatedFrame_;   
   int             beginSequence_;
   int             endSequence_;
   float           interpolateValue_;
   float           speedFactor_;
};


//
inline void MD2Animator
::setSpeedFactor( float speedFactor )
{
  speedFactor_ = speedFactor;
}

//
inline const MD2Data& MD2Animator
::parentMesh() const
{
  return meshData_;
}

} // namespace Framework
} // namespace Gcad

#endif

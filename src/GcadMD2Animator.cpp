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

#include "GcadMD2Animator.h"
#include "GcadMD2Data.h"

namespace {

enum SequenceScope {
  SS_START, /**< Okresla wartosc poczatkowa klatki sekwencji */
  SS_FINISH /**< Okresla wartosc koncowa klatki sekwencji */
};

// Funkcja spelniajaca wymagania dotyczace informacji okreslajacych
// poczatek, oraz zakonczenie danej sekwencji animacyjnej
int
md2ArraySequences( Gcad::Framework::MD2Animator::AnimSequence  sequence,
                   SequenceScope                    scope )
{
  const int START_FINISH             = 2;
  const int MD2_ANIM_SEQUENCES_COUNT = 20;

  static int md2AnimFramesScope[ MD2_ANIM_SEQUENCES_COUNT ][ START_FINISH ] = {
     0,   39,
     40,  45,
     46,  53,
     54,  57,
     58,  61,
     62,  65,
     66,  71,
     72,  83,
     84,  94,
     95, 111,
    112, 122,
    123, 134,
    135, 153,
    154, 159,
    160, 168,
    169, 172,
    173, 177,
    178, 183,
    184, 189,
    190, 197
  };

  return md2AnimFramesScope[ sequence ][ scope ];
}

//
inline int
md2BeginSequence( Gcad::Framework::MD2Animator::AnimSequence sequence )
{
  return md2ArraySequences( sequence, SS_START );
}

//
inline int
md2EndSequence( Gcad::Framework::MD2Animator::AnimSequence sequence )
{
  return md2ArraySequences( sequence, SS_FINISH );
}

} // anonymous

namespace Gcad {
namespace Framework {

//
MD2Animator
::MD2Animator( const MD2Data& parentMeshData )
  : meshData_( parentMeshData ),
    speedFactor_( 1 )
{
  setAnimation( AS_IDLE, AT_LOOP );
}

//
void MD2Animator
::setAnimation( AnimSequence  animationSequence,
                AnimType      animationType )
{
  beginSequence_ = md2BeginSequence( animationSequence );
  endSequence_   = md2EndSequence( animationSequence );

  firstInterpolatedFrame_  = beginSequence_;
  secondInterpolatedFrame_ = beginSequence_ + 1;

  currentAnimSeq_   = animationSequence;
  currentAnimType_  = animationType;
  interpolateValue_ = 0;
}

//
void MD2Animator
::update( float elapsedTime )
{
  interpolateValue_ += elapsedTime * speedFactor_;

  if( interpolateValue_ > 1.0 ) {
    ++firstInterpolatedFrame_;
    ++secondInterpolatedFrame_;
    interpolateValue_ = 0;
    
    if( firstInterpolatedFrame_ > endSequence_ &&
      currentAnimType_ == AT_SINGLE )
    {
      setAnimation( AS_IDLE, AT_LOOP );
    }
    else if( secondInterpolatedFrame_ > endSequence_ )
    {
      secondInterpolatedFrame_ = beginSequence_;
    }
    else if( firstInterpolatedFrame_ > endSequence_ &&
      currentAnimType_ == AT_LOOP )
    {
      setAnimation( currentAnimSeq_, AT_LOOP );
    }
  }
}

//
MD2Animator::VerticesKeyFrameAutoPtr MD2Animator
::interpolatedVertices() const
{
  const int VERTICES_COUNT = meshData_.attributes().numVerts();

  VerticesKeyFrameAutoPtr  vertices( new VerticesKeyFrame );

  vertices->reserve( VERTICES_COUNT );

  MD2Data::VerticesFrameConstItor  firstFrameItor =
    meshData_.beginVerticesKeyFrame( firstInterpolatedFrame_ );

  MD2Data::VerticesFrameConstItor  secondFrameItor =
    meshData_.beginVerticesKeyFrame( secondInterpolatedFrame_ );

  for( int verticeIndex = 0;
    verticeIndex < VERTICES_COUNT;
    ++verticeIndex )
  {
    vertices->push_back( *firstFrameItor +
      ( *secondFrameItor - *firstFrameItor ) * interpolateValue_ );

    ++firstFrameItor;
    ++secondFrameItor;
  }

  return vertices;
}

//
MD2Animator::NormalsKeyFrameAutoPtr MD2Animator
::interpolatedNormals() const
{
  const int VERTICES_COUNT = meshData_.attributes().numVerts();

  NormalsKeyFrameAutoPtr  normals( new NormalsKeyFrame );

  normals->reserve( VERTICES_COUNT );
  
  MD2Data::NormalsFrameConstItor  firstFrameItor =
    meshData_.beginNormalsKeyFrame( firstInterpolatedFrame_ );

  MD2Data::NormalsFrameConstItor  secondFrameItor =
    meshData_.beginNormalsKeyFrame( secondInterpolatedFrame_ );

  for( int verticeIndex = 0;
    verticeIndex < VERTICES_COUNT;
    ++verticeIndex )
  {
    normals->push_back( *firstFrameItor +
      ( *secondFrameItor - *firstFrameItor ) * interpolateValue_ );

    ++firstFrameItor;
    ++secondFrameItor;
  }

  return normals;
}

} // namespace Framework
} // namespace Gcad

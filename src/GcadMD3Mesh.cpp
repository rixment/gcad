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

#include "GcadMD3Mesh.h"
#include "GcadComputeModelNormals.h"

namespace {
  // Oryginalna reprezentacja struktury formatu MD3, wykorzystywana
  // w celu odczytania zawartosci dotyczacej wierzcholkow siatki
  struct Q3Vertice {
    signed short  vertex[ 3 ]; // wspolrzedne wierzcholka
    unsigned char normal[ 2 ]; // nie uzywane wartosci
  };
} // anonymous

namespace Gcad {
namespace Framework {

//
MD3Mesh
::MD3Mesh( std::istream& meshDataStream )
{
  std::istream::pos_type meshOffset = meshDataStream.tellg();

  meshHeader_ = new MD3MeshHeader( meshDataStream );

  readFacesIndices( meshDataStream, meshOffset );
  readTexCoords( meshDataStream, meshOffset );
  readVerticesKeyFrames( meshDataStream, meshOffset );

  computeVerticesNormals();
}

//
void MD3Mesh
::readFacesIndices( std::istream&           facesIndicesStream,
                    std::istream::pos_type  meshOffset )
{
  facesIndices_.resize( meshHeader_->trianglesNum() );

  facesIndicesStream.seekg( meshOffset );
  facesIndicesStream.seekg( 
    meshHeader_->trianglesStart(), 
    std::ios_base::cur );

  facesIndicesStream.read(
    reinterpret_cast< char* >( &facesIndices_.front() ),
    sizeof( FacesIndices::value_type ) * meshHeader_->trianglesNum() );
}

//
void MD3Mesh
::readTexCoords( std::istream&           texCoordsStream,
                 std::istream::pos_type  meshOffset )
{
  texCoords_.resize( meshHeader_->verticesNum() );
  
  texCoordsStream.seekg( meshOffset );
  texCoordsStream.seekg( 
    meshHeader_->uvStart(), 
    std::ios_base::cur );

  texCoordsStream.read(
    reinterpret_cast< char* >( &texCoords_.front() ),
    sizeof( TexCoords::value_type ) * meshHeader_->verticesNum() );
}

//
void MD3Mesh
::readVerticesKeyFrames( std::istream&           framesStream,
                         std::istream::pos_type  meshOffset )
{
  vertKeyFrames_.resize( meshHeader_->framesNum() );

  const int VERTICES_COUNT = meshHeader_->framesNum() * 
    meshHeader_->verticesNum();
  
  std::vector< Q3Vertice >            q3Vertices( VERTICES_COUNT );
  std::vector< Q3Vertice >::iterator  q3VertItor = q3Vertices.begin();

  framesStream.seekg( meshOffset );
  framesStream.seekg( 
    meshHeader_->vertexStart(), 
    std::ios_base::cur );
  
  framesStream.read(
    reinterpret_cast< char* >( &q3Vertices.front() ),
    sizeof( Q3Vertice ) * q3Vertices.size() );

  for( size_t frameIndex = 0;
    frameIndex < vertKeyFrames_.size();
    ++frameIndex )
  {
    vertKeyFrames_[ frameIndex ].resize( meshHeader_->verticesNum() );
    
    for( int vertexIndex = 0;
      vertexIndex < meshHeader_->verticesNum();
      ++vertexIndex )
    {
      const float Q3_SCALE_FACTOR = 64.0f;

      Vertices& currFrame = vertKeyFrames_[ frameIndex ];

      currFrame[vertexIndex].setX( q3VertItor->vertex[ 0 ] / Q3_SCALE_FACTOR );
      currFrame[vertexIndex].setY( q3VertItor->vertex[ 1 ] / Q3_SCALE_FACTOR );
      currFrame[vertexIndex].setZ( q3VertItor->vertex[ 2 ] / Q3_SCALE_FACTOR );

      ++q3VertItor;
    }   
  }
}

//
void MD3Mesh
::computeVerticesNormals()
{
  std::vector< int > polygonsIndices;
  for( FacesIndicesConstItor polyItor = beginFacesIndices();
    polyItor != endFacesIndices();
    ++polyItor )
  {
    polygonsIndices.push_back( ( *polyItor )[ 0 ] );
    polygonsIndices.push_back( ( *polyItor )[ 1 ] );
    polygonsIndices.push_back( ( *polyItor )[ 2 ] );
  }

  ComputeModelNormals  computeNormals(
    polygonsIndices.begin(),
    polygonsIndices.end(),
    meshHeader().verticesNum() );

  normKeyFrames_.resize( meshHeader_->framesNum() );

  for( size_t keyFrameIndex = 0;
    keyFrameIndex < vertKeyFrames_.size();
    ++keyFrameIndex )
  {
    computeNormals.computeFrameNormals(
      polygonsIndices.begin(),
      polygonsIndices.end(),
      vertKeyFrames_[ keyFrameIndex ],
      &normKeyFrames_[ keyFrameIndex ] );
  }
}

} // namespace Framework
} // namespace Gcad

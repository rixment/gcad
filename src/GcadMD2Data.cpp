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

#include "GcadMD2Data.h"
#include "GcadMD2Animator.h"
#include "GcadComputeModelNormals.h"
#include <fstream>
#include <memory>

namespace Gcad {
namespace Framework {

//
MD2Data
::MD2Data( const std::string& fileName )
  : fileName_( fileName )
  , fileHeader_( 0 )
{
  std::ifstream  md2DataStream( fileName_.c_str(), std::ios_base::binary );
  
  loadDataFromInput(md2DataStream);
}

//
MD2Data
::MD2Data( std::ifstream& inputData )
  : fileHeader_( 0 )
{
  loadDataFromInput(inputData);
}

//
void
MD2Data
::loadDataFromInput( std::ifstream& inputData )
{
  if( !inputData ) {
    throw FileReadError( "Blad podczas otwierania pliku " +  fileName_ );
  }

  fileHeader_ = MD2HeaderAutoPtr( new MD2FileHeader( inputData ) );

  BytesVectorAutoPtr dataFileBuffer( createBuffer( inputData ) );

  readVerticesKeyFrames( &dataFileBuffer->front() );
  readTextureCoords( &dataFileBuffer->front() );
  readPolygonsIndices( &dataFileBuffer->front() );    

  computeNormalsKeyFrames();
}

//
size_t 
MD2Data
::getByteSize() const
{
  return fileName_.size() + 
    sizeof(MD2FileHeader) + 
    vertFrames_.size() * sizeof(VerticesKeyFrames::value_type) +
    normFrames_.size() * sizeof(NormalsKeyFrames::value_type) +
    texCoords_.size() * sizeof(TextureCoords::value_type) +
    polyIndices_.size() * sizeof(PolygonsIndices::value_type);
}

//
void MD2Data
::computeNormalsKeyFrames()
{
  // Poniewaz obliczenie normalnych wierzcholkow bedzie oddelegowane
  // do obiektu typu ComputeModelNormals, nalezy w pierwszej kolejnosci
  // dostosowac strukture organizujaca wartosci poligonow do 
  // formatu wymaganego przez instancje klasy ComputeModelNormals

  std::vector< int > polygonsIndices;
  for( PolygonsIndicesConstItor polyIndexItor = beginPolygonsIndices();
    polyIndexItor != endPolygonsIndices();
    ++polyIndexItor )
  {
    polygonsIndices.push_back( polyIndexItor->indexToVerticesArray( 0 ) );
    polygonsIndices.push_back( polyIndexItor->indexToVerticesArray( 1 ) );
    polygonsIndices.push_back( polyIndexItor->indexToVerticesArray( 2 ) );
  }

  // Na tym etapie zostaje zbudowana tablica wartosci determinujaca
  // dla kazdego wierzcholka z osobna, jego przynaleznosc do danego poligonu

  ComputeModelNormals computeNormals( 
    polygonsIndices.begin(),
    polygonsIndices.end(),
    fileHeader_->numVerts() );

  // Aby wyliczyc normalne wierzcholkow potrzebnych w czasie obliczania
  // oswietlenia (dokladniej cieniowania Goureau) nalezy przetworzyc
  // wierzcholki kazdej kluczowej klatki animacji, z ktorych sklada sie model

  normFrames_.reserve( keyFramesCount() );

  for( size_t keyFrameIndex = 0;
    keyFrameIndex < keyFramesCount();
    ++keyFrameIndex )
  {
    normFrames_.push_back( new NormalsFrame );

    computeNormals.computeFrameNormals( 
      polygonsIndices.begin(),
      polygonsIndices.end(),
      *vertFrames_[ keyFrameIndex ],
      normFrames_[ keyFrameIndex ].get() );
  }
}

//
MD2Data::BytesVectorAutoPtr MD2Data
::createBuffer( std::istream& dataSrc ) 
{
  const int FILE_SIZE = fileHeader_->offsetEnd();
  BytesVectorAutoPtr dataFromFile( new BytesVector(FILE_SIZE) );

  const int MOVE_TO_BEGIN = 0;
  dataSrc.seekg( MOVE_TO_BEGIN, std::ios_base::beg );
  dataSrc.read( 
    reinterpret_cast< char* >( &dataFromFile->front() ),
    FILE_SIZE );
  
  // Ilosc wczytanych danych musi byc zgodna 
  // z wartoscia przesuniecia offsetEnd (rozmiar pliku)
  if( dataSrc.gcount() != FILE_SIZE ) {
    throw FileReadError("MD2Data: Blad podczas odczytu pliku " +  fileName_);
  }
  
  return dataFromFile;
}

//
void MD2Data
::readFrameVert( Byte*           verticesData,
                 VerticesFrame&  frameVert ) 
{
  // Wspolczynniki skalowania skompresowanych danych wierzcholkow 
  // w liczbie po 3 dla kazdej wartosci wspolrzednej osi xyz
  float  scale[ CB_SCALE ];
  float  translate[ CB_TRANSL ];
  Byte   name[ CB_STRING ];

  readFrameHeader( verticesData, scale, translate, name );
  
  int headerFrameOffset = sizeof( scale ) + sizeof( translate ) + sizeof( name );
  
  const int BLOCK_STEP = 4; // x, y, z, normal index
  for( int i = headerFrameOffset; 
    i < fileHeader_->frameSize(); 
    i += BLOCK_STEP ) 
  {
    frameVert.push_back( Vector3(
      verticesData[ i + 0 ] * scale[ 0 ] + translate[ 0 ],
      verticesData[ i + 1 ] * scale[ 1 ] + translate[ 1 ],
      verticesData[ i + 2 ] * scale[ 2 ] + translate[ 2 ] ) );
  }
}

//
void MD2Data
::readPolygonsIndices( Byte* fileData ) 
{
  const int POLYGON_DATA_SIZE = 
    fileHeader_->numPolys() * sizeof( PolygonIndices );

  Byte* polyData = fileData + fileHeader_->offsetPolys();
  polyIndices_.resize( fileHeader_->numPolys() );
  memcpy( &polyIndices_.front(), polyData, POLYGON_DATA_SIZE );
}
  
//
void MD2Data
::readTextureCoords( Byte* fileData ) 
{
  const int   TEX_COORD_NUM = fileHeader_->numTexCoords();
  const float SKIN_WIDTH    = static_cast< float >( fileHeader_->skinWidth() );
  const float SKIN_HEIGHT   = static_cast< float >( fileHeader_->skinHeight() );

  texCoords_.resize( TEX_COORD_NUM );

  short* texCoordsOffset =
    reinterpret_cast< short* >( fileData + fileHeader_->offsetTexCoords() );

  int currTexIndex = 0;
  for( short* currTex = texCoordsOffset;
    currTex < texCoordsOffset + ( TEX_COORD_NUM * 2 );
    ++currTex )
  {
    texCoords_[ currTexIndex ].setX( *currTex++ / SKIN_WIDTH );
    texCoords_[ currTexIndex ].setY( *currTex / SKIN_HEIGHT );
    ++currTexIndex;
  }
}

//
void MD2Data
::readVerticesKeyFrames( Byte* fileData ) 
{
  for( int frmeCount = 0; 
    frmeCount < fileHeader_->numFrames(); 
    ++frmeCount ) 
  {
    int frameOffset    = frmeCount * fileHeader_->frameSize();
    Byte* frmVertData  = fileData + fileHeader_->offsetFrames() + frameOffset;
    
    std::auto_ptr< VerticesFrame >  verticesFrame( new VerticesFrame );
    readFrameVert( frmVertData, *verticesFrame );
    vertFrames_.push_back( verticesFrame.release() );
  }
}

//
void MD2Data
::readFrameHeader( Byte* headerData,
                   float ( &scale )[ CB_SCALE ],
                   float ( &translate )[ CB_TRANSL ],
                   Byte ( &name ) [ CB_STRING ] ) 
{
  float* scaleData = reinterpret_cast< float* >( headerData );
  float* trnslData = scaleData + CB_SCALE;
  unsigned char* nameData   = reinterpret_cast< Byte* >( trnslData + CB_TRANSL );

  // Odczytanie danych naglowka klatki animacji
  memcpy( scale, scaleData, sizeof( scale ) );
  memcpy( translate, trnslData, sizeof( translate ) );
  memcpy( name, nameData, sizeof( name ) );
}

} // namespace Framework
} // namespace Gcad

// szukaj: "Triki najlepszych programistow gier 3D" strona 1130

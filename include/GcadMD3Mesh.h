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

#ifndef _GCAD_MD3MESH_H_
#define _GCAD_MD3MESH_H_

#include "GcadMD3MeshHeader.h"
#include "GcadVector2.h"
#include "GcadVector3.h"
#include "GcadRefCountPtr.h"
#include <iostream>
#include <vector>

namespace Gcad {
namespace Framework {

/**
  @brief
    Opis jednej siatki (podstawowego budulca modelu geometrycznego)
*/
class MD3Mesh {
 public:  
   typedef Gcad::Math::Vector2<float>  Vector2;
   typedef Gcad::Math::Vector3<float>  Vector3;

   /**
     @brief
       Klasa przechowuje trzy wartosci, ktore sluza jako indeksy
       tablic wierzcholkow, oraz koordynat tekstur, w procesie 
       renderingu siatki modelu
   */
   class Face {
    public:
      /**
        @brief
          Wyluskanie wartosci okreslajacych indeksy tablic wierzcholkow,
          oraz koordynat tekstur. Metoda przydatna w czasie renderingu
          siatki modelu
      */
      int operator[]( int index ) const {
        Gcad::Utilities::assertion( index >= 0 && index < 3,
          "MD3Mesh::Face::operator[](int): Indeks spoza zakresu!" );      
        return index_[ index ];
      }
    
    private: 
      int index_[ 3 ]; /**< @brief Wartosci wyznaczajace poligon siatki */
   };

   /**
     @brief Synonim inteligentnego wskaznika obiektu naglowka siatki
   */
   typedef Gcad::Utilities::RefCountPtr< MD3MeshHeader >  SmartPtrMeshHeader;

   /**
     @brief Synonim zestawu wierzcholkow reprezentujacych klatke kluczowa
   */
   typedef std::vector<Vector3>      Vertices;
   typedef Vertices::iterator        VerticesItor;
   typedef Vertices::const_iterator  VerticesConstItor;

   /**
     @brief Synonim zbioru wierzcholkow klatek kluczowych siatki
   */
   typedef std::vector< Vertices >  VerticesKeyFrames;

   /**
     @brief Synonim dla zestawu normalnych klatki kluczowej
   */
   typedef std::vector<Vector3>     Normals;
   typedef Normals::iterator        NormalsItor;
   typedef Normals::const_iterator  NormalsConstItor;
   
   /**
     @brief Synonim zbioru normalnych klatek kluczowych siatki
   */
   typedef std::vector< Normals >  NormalsKeyFrames;

   /**
     @brief Synonim kolekcji koordynat tekstur
   */
   typedef std::vector<Vector2>       TexCoords;
   typedef TexCoords::iterator        TexCoordsItor;
   typedef TexCoords::const_iterator  TexCoordsConstItor;
   
   /**
     @brief Synonim zbioru wartosci wykorzystywanych podczas okreslania poligonu
   */
   typedef std::vector< Face >           FacesIndices;
   typedef FacesIndices::iterator        FacesIndicesItor;
   typedef FacesIndices::const_iterator  FacesIndicesConstItor;

   /**
     @brief
       Konstruktor przeprowadzajacy inicjalizacje skladowych
       z wykorzystaniem danych pobieranych ze strumienia wejsciowego
   */
   MD3Mesh( std::istream& meshDataStream );

   /**
     @brief
       Akcesor skladowej opisujacej naglowek siatki (podstawowego
       budulca modelu MD3)
   */
   MD3MeshHeader meshHeader() const;

   /**
     @brief
       Udostepnienie iteratora do poczatku danych wierzcholkow
       klatki kluczowej wyszczegolnionej w argumencie wywolania
     
     @param
       frameIndex Wartosc determinujaca okreslona klatke kluczowa w sekwencji
   */
   VerticesItor       beginVerticesFrame( size_t frameIndex );
   VerticesItor       endVerticesFrame( size_t frameIndex );
   VerticesConstItor  beginVerticesFrame( size_t frameIndex ) const;
   VerticesConstItor  endVerticesFrame( size_t frameIndex ) const;

   /**
     @brief
   */
   NormalsItor       beginNormalsFrame( size_t frameIndex );
   NormalsItor       endNormalsFrame( size_t frameIndex );
   NormalsConstItor  beginNormalsFrame( size_t frameIndex ) const;
   NormalsConstItor  endNormalsFrame( size_t frameIndex ) const;

   /**
     @brief
   */
   FacesIndicesItor       beginFacesIndices();
   FacesIndicesItor       endFacesIndices();
   FacesIndicesConstItor  beginFacesIndices() const;
   FacesIndicesConstItor  endFacesIndices() const;

   /**
     @brief
   */
   TexCoordsItor       beginTexCoordsItor();
   TexCoordsItor       endTexCoordsItor();
   TexCoordsConstItor  beginTexCoordsItor() const;
   TexCoordsConstItor  endTexCoordsItor() const;

 private:
   void readFacesIndices( std::istream&           facesIndicesStream,
                          std::istream::pos_type  meshOffset );
   
   void readTexCoords( std::istream&           texCoordsStream,
                       std::istream::pos_type  meshOffset );

   void readVerticesKeyFrames( std::istream&           framesStream,
                               std::istream::pos_type  meshOffset );

   void computeVerticesNormals();

 private:   
   SmartPtrMeshHeader  meshHeader_;   /**< @brief Naglowek siatki modelu */
   VerticesKeyFrames   vertKeyFrames_;/**< @brief Zbior wierzcholkow klatek */
   NormalsKeyFrames    normKeyFrames_;/**< @brief Zbior normalnych klatek */
   TexCoords           texCoords_;    /**< @brief Koordynaty tekstur */
   FacesIndices        facesIndices_; /**< @brief Wskazniki wartosci poligonow */
};


//
inline MD3MeshHeader MD3Mesh
::meshHeader() const 
{ 
  return *meshHeader_; 
}

//
inline MD3Mesh::VerticesItor MD3Mesh
::beginVerticesFrame( size_t frameIndex )
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < vertKeyFrames_.size(),
    "MD3Mesh::beginVerticesFrame(size_t): Wartosc spoza zakresu!" );
  return vertKeyFrames_[ frameIndex ].begin();
}

//
inline MD3Mesh::VerticesConstItor MD3Mesh
::beginVerticesFrame( size_t frameIndex ) const
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < vertKeyFrames_.size(),
    "MD3Mesh::beginVerticesFrame(size_t): Wartosc spoza zakresu!" );
  return vertKeyFrames_[ frameIndex ].begin();
}

//
inline MD3Mesh::VerticesItor MD3Mesh
::endVerticesFrame( size_t frameIndex )
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < vertKeyFrames_.size(),
    "MD3Mesh::end(size_t): Wartosc spoza zakresu!" );
  return vertKeyFrames_[ frameIndex ].end();
}

//
inline MD3Mesh::VerticesConstItor MD3Mesh
::endVerticesFrame( size_t frameIndex ) const
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < vertKeyFrames_.size(),
    "MD3Mesh::end(size_t): Wartosc spoza zakresu!" );
  return vertKeyFrames_[ frameIndex ].end();
}

//
inline MD3Mesh::NormalsItor MD3Mesh
::beginNormalsFrame( size_t frameIndex )
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < normKeyFrames_.size(),
    "Wartosc spoza dozwolonego zakresu!" );
  return normKeyFrames_[ frameIndex ].begin();
}

//
inline MD3Mesh::NormalsConstItor MD3Mesh
::beginNormalsFrame( size_t frameIndex ) const
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < normKeyFrames_.size(),
    "Wartosc spoza dozwolonego zakresu!" );
  return normKeyFrames_[ frameIndex ].begin();
}

//
inline MD3Mesh::NormalsItor MD3Mesh
::endNormalsFrame( size_t frameIndex )
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < normKeyFrames_.size(),
    "Wartosc spoza dozwolonego zakresu!" );
  return normKeyFrames_[ frameIndex ].end();
}

//
inline MD3Mesh::NormalsConstItor MD3Mesh
::endNormalsFrame( size_t frameIndex ) const
{
  Gcad::Utilities::assertion( frameIndex >= 0 &&
    frameIndex < normKeyFrames_.size(), 
    "Wartosc spoza dozwolonego zakresu!" );
  return normKeyFrames_[ frameIndex ].end();
}

//
inline MD3Mesh::FacesIndicesItor MD3Mesh
::beginFacesIndices()
{
  return facesIndices_.begin();
}

//
inline MD3Mesh::FacesIndicesConstItor MD3Mesh
::beginFacesIndices() const
{
  return facesIndices_.begin();
}

//
inline MD3Mesh::FacesIndicesItor MD3Mesh
::endFacesIndices()
{
  return facesIndices_.end();
}

//
inline MD3Mesh::FacesIndicesConstItor MD3Mesh
::endFacesIndices() const
{
  return facesIndices_.end();
}

//
inline MD3Mesh::TexCoordsItor MD3Mesh
::beginTexCoordsItor()
{
  return texCoords_.begin();
}

//
inline MD3Mesh::TexCoordsConstItor MD3Mesh
::beginTexCoordsItor() const
{
  return texCoords_.begin();
}

//
inline MD3Mesh::TexCoordsItor MD3Mesh
::endTexCoordsItor()
{
  return texCoords_.end();
}

//
inline MD3Mesh::TexCoordsConstItor MD3Mesh
::endTexCoordsItor() const
{
  return texCoords_.end();
}

} // namespace Framework
} // namespace Gcad

#endif
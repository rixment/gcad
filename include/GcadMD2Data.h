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

#ifndef _GCAD_MD2DATA_H_
#define _GCAD_MD2DATA_H_

#include "GcadMD2FileHeader.h"
#include "GcadAssertion.h"
#include "GcadException.h"
#include "GcadRefCountPtr.h"
#include "GcadVector2.h"
#include "GcadVector3.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace Gcad {
namespace Framework {

/** 
  @brief
    Pomocnicza klasa udostepniajaca interfejs umozliwiajacy
    wczytanie danych z pliku formatu MD2 - Quake 2 
*/
class GCAD_EXPORT MD2Data {
 public:
   /** 
     @brief 
       Klasa obiektu wyjatku zaistnialego w przypadku powstania 
       bledu odczytu danych ze strumienia wejsciowego
   */
   class GCAD_EXPORT FileReadError : public Utilities::Exception {
     friend class MD2Data;
     FileReadError( const std::string& fileName )
       : Utilities::Exception( fileName )
     {}
   };

   /** 
     @brief  
       Definicja zawierajaca indeksy do tablic wartosci reprezentujacych
       podstawowy budulec siatki geometrycznej - trojkata
   */
   class GCAD_EXPORT PolygonIndices {
    public:
      /**
        @brief
          Udostepnienie wartosci determinujacej indeks tablicy
          wierzcholkow trojkata
      */
      unsigned short indexToVerticesArray( int index ) const {
        Utilities::assertion( index >= 0 && index < 3,
          "MD2Data::PolygonIndices::indexToVerticeArray(int): "
          "Wartosc spoza zakresu!" );
        return indexToVerticesArray_[ index ];
      }
      
      /**
        @brief
          Udostepnienie wartosci determinujacej indeks tablicy
          koordynat tekstur trojkata
      */
      unsigned short indexToTexCoordsArray( int index ) const {
        Utilities::assertion( index >= 0 && index < 3,
          "MD2Data::PolygonIndices::indexToTexCoordsArray(int): "
          "Wartosc spoza zakresu!" );
        return indexToTexCoordsArray_[ index ];
      }    

    private:
      /** 
        @brief 
          Trzy pary wartosci, kolejno dla kazdego wierzcholka trojkata,
          okreslajacych indeksy do tablicy wierzcholkow 
      */
      unsigned short  indexToVerticesArray_[ 3 ];
      
      /** 
        @brief 
          Trzy pary wartosci, kolejno dla kazdego wierzcholka trojkata,
          okreslajacych indeksy do tablicy koordynat tekstury 
      */
      unsigned short  indexToTexCoordsArray_[ 3 ];
   };

   /** 
     @brief Synonim "inteligentnego" wskaznika do instancji MD2FileHeader
   */
   typedef std::auto_ptr< MD2FileHeader >  MD2HeaderAutoPtr;

   /** 
     @brief 
       Synonim typu wartosci bufora danych MD2 tworzonego w celu
       wypelnienia danymi ze strumienia wejsciowego 
   */
   typedef unsigned char               Byte;
   typedef std::vector<Byte>           BytesVector;
   typedef std::auto_ptr<BytesVector>  BytesVectorAutoPtr;

   /**
     @brief 
       Synonimy obiektow matematycznych - wektorow wystepujacych w formie
       zespolu liczb zmiennopozycyjnych znajdujacych odwzorowanie w plikach
       formatu MD2
   */
   typedef Math::Vector2<float>  Vector2;
   typedef Math::Vector3<float>  Vector3;

   /** 
     @brief Synonim typu dla zbioru wierzcholkow jednej klatki animacji
   */
   typedef std::vector< Vector3 >                        VerticesFrame;
   typedef VerticesFrame::iterator                       VerticesFrameItor;
   typedef VerticesFrame::const_iterator                 VerticesFrameConstItor;
   typedef Gcad::Utilities::RefCountPtr< VerticesFrame > VerticesFrameSmartPtr;

   /** 
     @brief Synonim typu dla kolekcji klatek animacji wierzcholkow
   */
   typedef std::vector< VerticesFrameSmartPtr >  VerticesKeyFrames;
   typedef VerticesKeyFrames::iterator           VerticesKeyFramesItor;
   typedef VerticesKeyFrames::const_iterator     VerticesKeyFramesConstItor;

   /** 
     @brief Synonim typu dla zbioru normalnych jednej klatki animacji
   */
   typedef std::vector< Vector3 >                        NormalsFrame;
   typedef NormalsFrame::iterator                        NormalsFrameItor;
   typedef NormalsFrame::const_iterator                  NormalsFrameConstItor;
   typedef Gcad::Utilities::RefCountPtr< NormalsFrame >  NormalsFrameSmartPtr;

   /** 
     @brief Synonim typu dla kolekcji klatek animacji normalnych
   */
   typedef std::vector< NormalsFrameSmartPtr >  NormalsKeyFrames;
   typedef NormalsKeyFrames::iterator           NormalsKeyFramesItor;
   typedef NormalsKeyFrames::const_iterator     NormalsKeyFramesConstItor;

   /** 
     @brief Synonim koordynat tekstur wierzcholka 
   */
   typedef std::vector< Vector2 >         TextureCoords;
   typedef TextureCoords::iterator        TextureCoordsItor;
   typedef TextureCoords::const_iterator  TextureCoordsConstItor;

   /** 
     @brief 
       Synonim kolekcji obiektow, opisujacych poligony 
       za posrednictwem wartosci okreslajacych indeksy w tablicy
       wierzcholkow i koordynat tekstur modelu
   */
   typedef std::vector< PolygonIndices >    PolygonsIndices;
   typedef PolygonsIndices::iterator        PolygonsIndicesItor;
   typedef PolygonsIndices::const_iterator  PolygonsIndicesConstItor;
   
   /** 
     @brief 
       Konstruktor obiektu pobierajacy dane ze zrodla plikowego
       okreslonego poprzez argument wywolania o identyfikatorze fileName

     @exception
       FileReadError
   */
   MD2Data( const std::string& fileName );

   /**
     @brief 
       Odczyt danych MD2 z podanego zewnetrznie zrodla. Zadany strumien
       powinien cechowac sie wartoscia ios::binary
   */
   MD2Data( std::ifstream& inputData );
  
   /** 
     @brief Udostepnienie informacji dotyczacych naglowka wczytanego pliku
   */
   MD2FileHeader attributes() const; 

   /**
   */
   size_t getByteSize() const;

   /** 
     @brief 
       Udostepnienie danych wierzcholkow jednej klatki animacji 
       okreslonej przez parametr wywolania metody
   */
   VerticesFrameItor       beginVerticesKeyFrame( size_t frameNum );
   VerticesFrameItor       endVerticesKeyFrame( size_t frameNum );
   VerticesFrameConstItor  beginVerticesKeyFrame( size_t frameNum ) const;
   VerticesFrameConstItor  endVerticesKeyFrame( size_t frameNum ) const;

   /** 
     @brief 
       Udostepnienie danych normalnych jednej klatki animacji 
       okreslonej przez parametr wywolania metody
   */
   NormalsFrameItor       beginNormalsKeyFrame( size_t frameNum );
   NormalsFrameItor       endNormalsKeyFrame( size_t frameNum );
   NormalsFrameConstItor  beginNormalsKeyFrame( size_t frameNum ) const;
   NormalsFrameConstItor  endNormalsKeyFrame( size_t frameNum ) const;

   /**
     @brief Informacja dotyczaca ilosci klatek kluczowych
   */
   size_t keyFramesCount() const;

   /** 
     @brief 
       Udostepnienie danych opisujacych koordynaty przypadajace na wierzcholek
   */
   TextureCoordsItor       beginTextureCoords();
   TextureCoordsItor       endTextureCoords();
   TextureCoordsConstItor  beginTextureCoords() const;
   TextureCoordsConstItor  endTextureCoords() const;

   /** 
     @brief Udostepnienie opisu poligonow 
   */
   PolygonsIndicesItor       beginPolygonsIndices();
   PolygonsIndicesItor       endPolygonsIndices();
   PolygonsIndicesConstItor  beginPolygonsIndices() const;
   PolygonsIndicesConstItor  endPolygonsIndices() const;

 private:
   /**
     @brief 
       Wykonanie odczytu danych formatu MD2 z zadanego strumienia wejsciowego
   */
   void loadDataFromInput( std::ifstream& inputData );

   /** 
     @brief 
       Utworzenie bufora o wielkosci rownej wartosci pola struktury 
       MD2FileHeader::offsetEnd, oraz wypelnienie go zawartoscia pliku 
   */
   BytesVectorAutoPtr createBuffer( std::istream& dataSrc );

   /** 
     @brief Odczyt wartosci reprezentujacych pojedyncza klatke animacji 
   */
   void readFrameVert( Byte*           verticesData, 
                       VerticesFrame&  frameVert );

   /** 
     @brief 
       Wartosci opisujace ilosc bajtow przypisanych do kazdej
       danej skladowej, ktora zawiera sie w pliku, a dokladniej 
       w naglowku klatki animacji 
   */
   enum { 
     CB_SCALE  = 3,  /**< @brief Czynnik skali dla osi x y z */
     CB_TRANSL = 3,  /**< @brief Czynnik przesuniecia */
     CB_STRING = 16  /**< @biref Ciag znakow definiujacy nazwe klatki animacji */
   }; 

   /** 
     @brief Odczyt wartosci towarzyszacych kazdej klatce animacji 
   */
   void readFrameHeader( Byte* headerData,
                         float ( &scale )[ CB_SCALE ],
                         float ( &translate )[ CB_TRANSL ],
                         Byte  ( &name )[ CB_STRING ] );
                             
   void readVerticesKeyFrames( Byte* fileData );
   void readTextureCoords( Byte* fileData );
   void readPolygonsIndices( Byte* fileData );

   void computeNormalsKeyFrames();

 private:
   std::string        fileName_;   /**< @brief Nazwa pliku zrodla danych */
   MD2HeaderAutoPtr   fileHeader_; /**< @brief Dane naglowka pliku */

   VerticesKeyFrames vertFrames_;  /**< @brief Wierzcholki klatek animacji */
   NormalsKeyFrames  normFrames_;  /**< @brief Normalne wierzcholkow klatek */
   TextureCoords     texCoords_;   /**< @brief Koordynaty tekstur wierzcholkow */
   PolygonsIndices   polyIndices_; /**< @brief Wartosci determinujace poligony */
};


//
inline MD2FileHeader MD2Data
::attributes() const 
{
  return *fileHeader_;
}

//
inline MD2Data::VerticesFrameItor MD2Data
::beginVerticesKeyFrame( size_t frameNum )
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < vertFrames_.size(),
    "MD2Data::beginVerticesKeyFrame(size_t): Indeks spoza zakresu!" );
  return vertFrames_[ frameNum ]->begin();
}

//
inline MD2Data::VerticesFrameItor MD2Data
::endVerticesKeyFrame( size_t frameNum )
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < vertFrames_.size(),
    "MD2Data::endVerticesKeyFrame(size_t): Indeks spoza zakresu!" );
  return vertFrames_[ frameNum ]->end();
}

//
inline MD2Data::VerticesFrameConstItor MD2Data
::beginVerticesKeyFrame( size_t frameNum ) const 
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < vertFrames_.size(),
    "MD2Data::beginVerticesKeyFrame(size_t): Indeks spoza zakresu!" );
  return vertFrames_[ frameNum ]->begin();
}

//
inline MD2Data::VerticesFrameConstItor MD2Data
::endVerticesKeyFrame( size_t frameNum ) const 
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < vertFrames_.size(),
    "MD2Data::endVerticesKeyFrame(size_t): Indeks spoza zakresu!" );
  return vertFrames_[ frameNum ]->end();
}

//
inline MD2Data::NormalsFrameItor MD2Data
::beginNormalsKeyFrame( size_t frameNum )
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < normFrames_.size(),
    "MD2Data::beginNormalsKeyFrame(size_t): Indeks spoza zakresu!" );
  return normFrames_[ frameNum ]->begin();
}

//
inline MD2Data::NormalsFrameItor MD2Data
::endNormalsKeyFrame( size_t frameNum )
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < normFrames_.size(),
    "MD2Data::endNormalsKeyFrame(size_t): Indeks spoza zakresu!" );
  return normFrames_[ frameNum ]->end();
}

//
inline MD2Data::NormalsFrameConstItor MD2Data
::beginNormalsKeyFrame( size_t frameNum ) const 
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < normFrames_.size(),
    "MD2Data::beginNormalsKeyFrame(size_t): Indeks spoza zakresu!" );
  return normFrames_[ frameNum ]->begin();
}

//
inline MD2Data::NormalsFrameConstItor MD2Data
::endNormalsKeyFrame( size_t frameNum ) const 
{
  Gcad::Utilities::assertion( frameNum >= 0 &&
    frameNum < normFrames_.size(),
    "MD2Data::endNormalsKeyFrame(size_t): Indeks spoza zakresu!" );
  return normFrames_[ frameNum ]->end();
}

//
inline size_t MD2Data
::keyFramesCount() const
{
  return vertFrames_.size();
}

//
inline MD2Data::TextureCoordsItor MD2Data
::beginTextureCoords() 
{
  return texCoords_.begin();
}

//
inline MD2Data::TextureCoordsItor MD2Data
::endTextureCoords() 
{
  return texCoords_.end();
}

//
inline MD2Data::TextureCoordsConstItor MD2Data
::beginTextureCoords() const 
{
  return texCoords_.begin();
}

//
inline MD2Data::TextureCoordsConstItor MD2Data
::endTextureCoords() const 
{
  return texCoords_.end();
}

//
inline MD2Data::PolygonsIndicesItor MD2Data
::beginPolygonsIndices() 
{
  return polyIndices_.begin();
}

//
inline MD2Data::PolygonsIndicesItor MD2Data
::endPolygonsIndices() 
{
  return polyIndices_.end();
}

//
inline MD2Data::PolygonsIndicesConstItor MD2Data
::beginPolygonsIndices() const 
{
  return polyIndices_.begin();
}

//
inline MD2Data::PolygonsIndicesConstItor MD2Data
::endPolygonsIndices() const 
{
  return polyIndices_.end();
}

} // namespace Framework
} // namespace Gcad

#endif

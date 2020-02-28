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

#ifndef _GCAD_COMPUTEMODELNORMALS_H_
#define _GCAD_COMPUTEMODELNORMALS_H_

#include "GcadBase.h"
#include "GcadAssertion.h"
#include "GcadRefCountPtr.h"
#include "GcadVector3.h"
#include "GcadVector3Util.h"
#include <vector>

namespace Gcad {
namespace Framework {

/**
  @brief
    Instancje klasy wykorzystywane sa do obliczania wektorow 
    normalnych wierzcholkow, potrzebnych przy obliczaniu oswietlenia
    typu Goureau

  @remark
    Instancje sa wykorzystwane w obliczeniach normalnych siatek 
    formatu MD2 (core::MD2Data), oraz MD3 (core::MD3Mesh)
*/
class GCAD_EXPORT ComputeModelNormals {
 public:
   typedef Gcad::Math::Vector3<float>  Vector3;

   /**
     @brief Zestaw wierzcholkow kluczowej klatki animacji
   */
   typedef std::vector< Vector3 >  VerticesFrame;
   
   /**
     @brief Zestaw normalnych kluczowej klatki animacji
   */   
   typedef std::vector< Vector3 >                        NormalsFrame;
   typedef Gcad::Utilities::RefCountPtr< NormalsFrame >  NormalsFrameSmartPtr;

   /**
     @brief
       Wzorzec konstruktora, ktory tworzy tablice przegladowa dla 
       zestawu wierzcholkow
       
       Wzorzec konstruktora, ktory tworzy tablice przegladowa dla 
       zestawu wierzcholkow. Dla kolejnych wierzcholkow sekwencji, zostaja 
       przyporzadkowane wartosci indeksow tablicy poligonow 
       (wierzcholek moze byc wkomponowany w kilka poligonow, trojkatow)

     @param
       begin Argument okreslajacy poczatek zestawu "wskaznikow" (wartosci
       typu calkowitego) do tablicy wierzcholkow. Kazde kolejne trzy wartosci
       definiuja jeden trojkat (zbudowany z trzech wierzcholkow)

     @param
       end Argument okreslajacy koniec zestawu "wskaznikow" do tablicy 
       wierzcholkow

     @param
       keyFrameVerticesCount Wartosc determinujaca ilosc wierzcholkow
       przypadajaca na jedna kluczowa klatke animacji
   */
   template< typename POLYGON_INDICES_ITOR >
   ComputeModelNormals( POLYGON_INDICES_ITOR  begin,
                        POLYGON_INDICES_ITOR  end,
                        int                   keyFrameVerticesCount );
 
   /**
     @brief
       Metoda wyliczajaca wartosci normalnych dla wierzcholkow siatki

       Zadaniem metody jest ustalenie (na podstawie przekazanych, oraz
       wczesniej wyliczonych wartosci w konstruktorze) wektorow dla
       kazdego wierzcholka animacji. W wyniku przetworzenia danych,
       zostaje utworzona tablica aproksymowanych wektorow normalnych,
       sprzezonych z wyszczegolnionymi wierzcholkami siatki animacyjnej

     @param 
       begin Poczatek zakresu danych "wskaznikowych" poligonow
     
     @param 
       begin Koniec zakresu danych "wskaznikowych" poligonow

     @param
       verticesKeyFrame Sprawdzajac typ argumentu mozemy dojsc do wniosku,
       iz argumentem wywolania jest referencja do zestawu wierzcholkow
       konkretnej klatki kluczowej animacji

     @param
       normalsFrame Argumentem jest wskaznik do zestawu normalnych 
       wierzcholkow. Po zakonczeniu wykonywania metody obiekt, ktorego 
       adres zostaje przekazany magazynowac bedzie skonstruowane
       wartosci determinujace normalne wierzcholkow.<br>
       Funkcja korzysta z metody std::vector::swap(), czego implikacja
       jest usuniecie zawartosci danych skojarzonych z kontenerem bedacym
       argumentem jej wywolania
   */
   template< typename POLYGON_INDICES_ITOR >
   void computeFrameNormals( POLYGON_INDICES_ITOR  begin,
                             POLYGON_INDICES_ITOR  end,
                             const VerticesFrame&  verticesKeyFrame,
                             NormalsFrame*         normalsFrame );

 private:
   /**
     @brief 
       Zamienna nazwa dla typu determinujacego zestaw wartosci
       skojarzonych z wierzcholkiem, ktore okreslaja jego przynaleznosc
       do wyszczegolnionuch poligonow (sa to wartosci indeksow do
       tablicy poligonow )
   */
   typedef std::vector< int >            IndicesToPolyArray;
   typedef IndicesToPolyArray::iterator  IndicesToPolyArrayItor;
   
   /**
     @brief 
       Informacje dotyczace poligonow dla wszystkich wierzcholkow siatki
   */
   typedef std::vector< IndicesToPolyArray >  VertexPolygonsIndices;

   /**
     @brief Synonim normalnych poligonow
   */   
   typedef std::vector< Vector3 >  
     PolygonsNormals;
   typedef Gcad::Utilities::RefCountPtr< PolygonsNormals >  
     PolygonsNormalsSmartPtr;

 private:
   /**
     @brief
       Hermetyzacja funkcji obliczenia normalnych poligonow
   */
   template< typename POLYGON_INDICES_ITOR >
   PolygonsNormalsSmartPtr computePolyNormals( POLYGON_INDICES_ITOR  begin,
                                               POLYGON_INDICES_ITOR  end,
                                               const VerticesFrame&  frame );

   /**
     @brief
       Hermetyzacja obliczenia normalnych dla wierzcholkow
   */
   NormalsFrameSmartPtr 
     computeAproximatedNormals( PolygonsNormalsSmartPtr polygonsNormals );

 private:
   /**
     @brief 
       Zestaw wartosci, kolejno dla kazdego z wierzcholkow, okreslajacych
       poligony, do ktorych przynaleza
   */
   VertexPolygonsIndices  polyIndicesForVertices_; 
};


//
template< typename POLYGON_INDICES_ITOR > ComputeModelNormals
::ComputeModelNormals( POLYGON_INDICES_ITOR  begin,
                       POLYGON_INDICES_ITOR  end,
                       int                   keyFrameVerticesCount )
  : polyIndicesForVertices_( keyFrameVerticesCount ) 
{
  Gcad::Utilities::assertion( keyFrameVerticesCount != 0, 
    "Siatka nie moze skladac sie z zera wierzcholkow!" );

  Gcad::Utilities::assertion( begin != end, 
    "Kolekcja indeksow do tablicy poligonow nie moze byc pusta!" );

  int                   currPolygonIndex = 0;
  POLYGON_INDICES_ITOR  currPolygonItor  = begin;
    
  while( currPolygonItor != end ) 
  {
    // Okreslenie wartosci indeksow do tablicy wierzcholkow modelu.
    // Za ich posrednictwem mozliwe jest odwzorowanie jednego
    // trojkata siatki. Jest to typowa metoda wykorzystywana podczas 
    // renderingu siatki modelu. Operuje ona na strukturze listy trojkatow

    const int FIRST_VERTICE_INDEX = *currPolygonItor++;
    const int SECON_VERTICE_INDEX = *currPolygonItor++;
    const int THIRD_VERTICE_INDEX = *currPolygonItor++;

    polyIndicesForVertices_[ FIRST_VERTICE_INDEX ].push_back( currPolygonIndex );
    polyIndicesForVertices_[ SECON_VERTICE_INDEX ].push_back( currPolygonIndex );
    polyIndicesForVertices_[ THIRD_VERTICE_INDEX ].push_back( currPolygonIndex );
    
    ++currPolygonIndex;
  }
}

//
template< typename POLYGON_INDICES_ITOR > void ComputeModelNormals
::computeFrameNormals( POLYGON_INDICES_ITOR  begin,
                       POLYGON_INDICES_ITOR  end,
                       const VerticesFrame&  verticesKeyFrame,
                       NormalsFrame*         normalsFrame )
{
  Gcad::Utilities::assertion( begin != end, 
    "Kolekcja indeksow do tablicy wierzcholkow nie moze byc pusta!" );

  Gcad::Utilities::assertion( verticesKeyFrame.empty() == false,
    "Klatka kluczowa musi zawierac dane!" );

  Gcad::Utilities::assertion( normalsFrame != 0, 
    "Blednie ustawiony wskaznik normalnych wierzcholkow" );

  // Zestaw normalnych poligonow. Zakonczenie etapu wyliczania normalnych
  // poligonow implikuje mozliwosc obliczania cieniowania plaskiego.
  // Za posrednictwem wektorow normalnych obliczanych dla kazdej kluczowej
  // klatki bedzie mozna obliczyc ich ursrednione kompozycje przynalezne
  // do poszczegolnych wierzcholkow modelu

  PolygonsNormalsSmartPtr  polygonsNormals = 
    computePolyNormals( begin, end, verticesKeyFrame );

  // Wykonanie glownego dzialania, ktore reprezentowane jest przez 
  // klase - obliczenie normalnych dla wierzcholkow
 
  NormalsFrameSmartPtr  verticesNormals =
    computeAproximatedNormals( polygonsNormals );
 
  normalsFrame->swap( *verticesNormals );
}

//
template< typename POLYGON_INDICES_ITOR > 
ComputeModelNormals::PolygonsNormalsSmartPtr ComputeModelNormals
::computePolyNormals( POLYGON_INDICES_ITOR  begin,
                      POLYGON_INDICES_ITOR  end,
                      const VerticesFrame&  frame )
{
  PolygonsNormalsSmartPtr  polygonsNormals = new PolygonsNormals;
  
  POLYGON_INDICES_ITOR  curr = begin;

  while( curr != end ) {
    // Determinacja trzech wspolrzednych determinujacych 
    // wierzcholki przetwarzanego trojkata
    const Vector3 triangle[ 3 ] = {
      frame[ *curr++ ],
      frame[ *curr++ ],
      frame[ *curr++ ]
    };
    
    Vector3  tangent ( triangle[ 1 ] - triangle[ 0 ] );
    Vector3  binormal( triangle[ 1 ] - triangle[ 2 ] );
    Vector3  normal;

    Gcad::Math::Vector3Util::cross( tangent, binormal, &normal );
    Gcad::Math::Vector3Util::normalize( normal, &normal );

    polygonsNormals->push_back( normal );
  }
  return polygonsNormals;
}

} // namespace Framework
} // namespace Gcad

#endif
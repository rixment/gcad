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

#ifndef _GCAD_CENTEROFMODEL_H_
#define _GCAD_CENTEROFMODEL_H_

namespace Gcad {
namespace Math {

/** 
  @brief
    Struktura definiujaca metode wyliczania srodka modelu 
    reprezentowanego zarowno w dwu- jak i troj- wymiarowej
    przestrzeni

  @code
    #include "GcadVector3.h"
    #include "GcadCenterOfModel.h"
    #include <vector>

    using namespace Gcad::Math;
    using namespace std;

    int main()
    {
      typedef Vector3<double> MathVector;
      
      vector< MathVector >  modelVertices;

      // Wypelnienie kontenera danymi modelu

      MathVector modelCenter = 
        CenterOfModel::compute< MathVector >( modelVertices.begin(),
                                              modelVertices.end() );
    }
  @endcode
*/
struct CenterOfModel {

  /** 
    @brief 
      Obliczenie srodka modelu

    @remark 
      Wymiar modelu determinowany jest przez parametr
      szablonu VECTOR_DIMM, ktory przyjac powinien jedna z wartosci:
        - Gcad::Math::Vector2<>
        - Gcad::Math::Vector3<>

    @return 
      Wektor determinujacy srodek modelu geometrycznego

    @param 
      beginScopeVector Wskaznik, badz iterator do poczatku
      kolekcji wierzcholkow definiujacych model

    @param 
      endScopeVector Oznaczenie konca kolekcji zgodnego
      z polityka standardu STL
  */
  template< typename VECTOR_DIMM, typename ITOR > 
  static VECTOR_DIMM
    compute( ITOR  beginScopeVector,
             ITOR  endScopeVector );

};


//
template< typename VECTOR_DIMM, typename ITOR > 
VECTOR_DIMM 
CenterOfModel
::compute( ITOR  beginScopeVector,
           ITOR  endScopeVector )
{
  // Aby wyliczyc srodek modelu nalezy zsumowac wszystkie wartosci
  // wierzcholkow, z ktorych jest zbudowany. Otrzymana wartosc trzba
  // nastepnie podzielic przez ilosc wierzcholkow tworzacych model

  int verticesCount = 0;
  VECTOR_DIMM  centerOfModel;

  for( ITOR currVertItor = beginScopeVector;
    currVertItor != endScopeVector;
    ++currVertItor )
  {
    centerOfModel += *currVertItor;
    ++verticesCount;
  }
  
  return centerOfModel /= verticesCount;
}

} // namespace Math
} // namespace Gcad

#endif
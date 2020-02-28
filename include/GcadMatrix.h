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

#ifndef _GCAD_MATRIX_H_
#define _GCAD_MATRIX_H_

#include "GcadAssertion.h"
#include <cstddef>

namespace Gcad {
namespace Math {

/** 
  @brief
    Reprezentacja matematycznego obiektu - macierzy, zaimplementowanego
    z wykorzystaniem statycznych tablic o rozmiarze dedukowanym 
    poprzez parametry szablonu
*/
template< int ROWS, 
          int COLS, 
          typename T > 
class Matrix {
 public:
   typedef T          value_type;
   typedef T*         iterator;
   typedef const T*   const_iterator;
   typedef T&         reference;
   typedef const T&   const_reference;
   typedef size_t     size_type;
   typedef ptrdiff_t  difference_type;

   /** 
     @brief 
       Domyslny konstruktor

     @remark 
       Dedukowany typ T powinien posiadac konstruktor domniemany, aby 
       domyslny konstruktor macierzy mogl zostac prawidlowo skonkretyzowany.
       Dla obiektow typu podstawowego wartosci elementow po wywolaniu 
       konstruktora sa <b>niezdefiniowane!</b>
   */
   Matrix();
   
   /** 
     @brief 
       Konstruktor przyjmujacy wartosci zawarte w zakresie podanym przez
       pare iteratorow

     @param 
       leftIter Iterator wskazujacy na poczatek zakresu danych

     @param 
       rightIter Iterator wskazujacy na pozycje za ostatnim elementem
       wyznaczajacym zasieg danych

     @remark 
       Konstruktor jest malo wydajny, poniewaz w pierwszej kolejnosci
       zostaje wywolywany konstruktor domyslny dla obiektow (ROWS * COLS) 
       typu dedukowanego parametrem T, a nastepnie wykonywana jest operacja
       przypisania poczynajac od pierwszego do ostatniego elementu, co 
       jest rownoznaczne stwierdzeniu, iz dla typow pierwszo-klasowych
       sa wywolywane kolejno konstruktor, oraz operator przypisania
       implikujac wywolanie destruktora, oraz konstruktora kopiujacego
   */
   template< typename ITER >
     Matrix( ITER  leftIter, 
             ITER  rightIter );
  
   /** 
     @brief 
       Sprowadzenie macierzy do formy pierwotnej

     @remark 
       Dla typow wbudowanych okreslenie "pierwotnej" jest 
       rownoznaczne macierzy zerowej. Dla typow zdefiniowanych
       przez uzytkownika - wywolanie destruktorow dla wszystkich elementow 
       znajdujacych sie w macierzy, a nastepnie wywolanie konstruktorow 
       domyslnych
   */
   void restore();

   /** 
     @brief Operacja wyluskania wiersza o zadanej wartosci indeksu
   */
   T* operator []( int rowIndex ); 
   
   /**
     @brief Operacja wyluskania wiersza o zadanej wartosci indeksu
   */
   const T* operator []( int rowIndex ) const;

   /** 
     @brief 
       Udostepnia klientowi informacje na temat 
       szerokosci (ilosc kolumn) danego zkonkretyzowanego wzorca
   */
   int width() const;
   
   /** 
     @brief Udostepnia klientowi informacje na temat 
       wysokosci (ilosc wierszy) danego zkonkretyzowanego wzorca
   */
   int height() const;

   /** 
     @brief 
       Zwraca adres pamieci pierwszego elementu macierzy

     @remark 
       Wykorzystujac metode <i>begin()</i> oraz <i>end()</i> 
       mozna w latwy sposob wykonac iteracje poprzez wszystkie 
       elementy macierzy.
   */
   T* begin();

   /** 
     @brief 
       Zwraca adres pamieci okreslajacy kolejny element za ostatnim 
       macierzy. Metoda przydatna podczas okreslania zakresu elementow macierzy
   */
   T* end();

   /** 
     @see begin()
   */
   const T* begin() const;

   /** 
     @see end()
   */
   const T* end() const;

 private:
   T  table_[ ROWS ][ COLS ]; 
};

template<typename T>
class Matrix<0,0,T> {};


template< int ROWS, int COLS, typename T > 
Matrix< ROWS, COLS, T >
::Matrix() 
{
  Utilities::assertion( ROWS != 0 && COLS != 0,
    "Bledne wartosci okreslajace wymiary macierzy" );
}

template< int ROWS, int COLS, typename T > 
template< typename ITER > 
Matrix< ROWS, COLS, T >
::Matrix( ITER  leftIter,
          ITER  rightIter )
{ 
  Utilities::assertion( leftIter != rightIter,
    "Iteratory wyznaczaja bledny zakres" );

  const int MAX_MATRIX_ELEMENTS = ROWS * COLS;
  int elemsCounter = 0;
  T* const initPtr = *table_;
  
  while( leftIter != rightIter && 
    elemsCounter < MAX_MATRIX_ELEMENTS )
  {
    initPtr[ elemsCounter++ ] = *leftIter++;
  }
  
  Utilities::assertion( elemsCounter <= MAX_MATRIX_ELEMENTS,
    "Wewnetrzny blad ujawniony podczas inicjalizacji macierzy!" );
}

template< int ROWS, int COLS, typename T > 
void 
Matrix< ROWS, COLS, T >
::restore() 
{
  const int  LAST_ELEMENT_INDEX = ROWS * COLS - 1;
  T* const   singleDimensionPtr = *table_;
  
  for( int elem = LAST_ELEMENT_INDEX; elem >= 0; --elem )
    singleDimensionPtr[ elem ].T::~T();
  
  for( int elem = 0; elem < ROWS * COLS; ++elem )
    new ( singleDimensionPtr + elem ) T;
}

template< int ROWS, int COLS, typename T > 
T* 
Matrix< ROWS, COLS, T >
::operator []( int rowIndex )
{
  Utilities::assertion( rowIndex < ROWS,
    "Indeks spoza zakresu!" );

  return table_[ rowIndex ];
}

template< int ROWS, int COLS, typename T > 
const T* 
Matrix< ROWS, COLS, T >
::operator []( int rowIndex ) const 
{
  Matrix* This = const_cast< Matrix* >( this );
  return ( *This )[ rowIndex ];
}

template< int ROWS, int COLS, typename T > 
int 
Matrix< ROWS, COLS, T >
::width() const 
{
  return COLS;
}

template< int ROWS, int COLS, typename T > 
int 
Matrix< ROWS, COLS, T >
::height() const 
{
  return ROWS;
}

template< int ROWS, int COLS, typename T > 
T* 
Matrix< ROWS, COLS, T >
::begin()
{
  return table_[ 0 ];
}

template< int ROWS, int COLS, typename T > 
T* 
Matrix< ROWS, COLS, T >
::end()
{
  return table_[ height() - 1 ] + width();
}

template< int ROWS, int COLS, typename T > 
const T* 
Matrix< ROWS, COLS, T >
::begin() const
{
  return table_[ 0 ];
}

template< int ROWS, int COLS, typename T > 
const T* 
Matrix< ROWS, COLS, T >
::end() const
{
  return table_[ height() - 1 ] + width();
}

} // namespace Math
} // namespace Gcad

#endif

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

#ifndef _GCAD_DYNAMICMATRIX_H_
#define _GCAD_DYNAMICMATRIX_H_

#include "GcadAssertion.h"
#include <algorithm>

namespace Gcad {
namespace Utilities {

/**
  @brief
    Dynamicznie powiekszana struktura organizacyjna zbioru elementow, 
    reprezentowana przez interfejs determinujacy dwuwymiarowa tablice -
    macierz

    Klasa definiuje implementacje macierzy ogolnego przeznaczenia. 
    Zasoby pamieci przeznaczone na elementy danego typu T sa 
    przydzielane dynamicznie. Interfejs publiczny, definiujacy zachowania, 
    oraz wlasciwosci klasy jednoznacznie utozsamia ja z pojeciem kontenera.
    Powstale skojarzenie implikuje nasuwajace sie pytanie: czy agregat posiada
    prawo wlasnosci zawartych w nim elementow? Odpowiedz jest rownoznaczna 
    ze schematem zastosowanym w bibliotece standardowej C++ - zeby byc scislym,
    czesci odpowiedzialnej za definicje rodziny kontenerow, zawartej w Standard 
    Template Library - ktora stosuje semantyke wartosci. W wyniku agregowany
    typ elementow jest zobowiazany do posiadania definicji metod kopiujacych
    (operatora przypisania, oraz konstruktora kopiujacego). Jest to elementarne 
    wymaganie stawiane parametryzowanym typom, ktorych instancje zostaja 
    wkomponowane w instancjach kontenerowych STL. Reasumujac - klasa 
    DynamicMatrix utozsamia swoje zachowania do regul przyjetych w implementacji
    kontenerow STL
  
  @remark 
    Typ nie powinien byc wykorzystywany jako superklasa. Jawne 
    wyszczegolnienie tego faktu mozna odczytac z definicji klasy, w ktorej
    brak jest wirtualnego destrutkora<br>
    Jesli w obiekcie typu DynamicMatrix sa przechowywane wskazniki na obiekty
    przydzielone na stercie, wowczas za poprawna dezalokacje odpowiada klient.
    Efektywna, oraz wolna od bledow technika zawierania, odmienna od 
    metody przechowywania "nagich wskaznikow", to uzycie wskaznikow obiektow,
    posiadajacych dodatkowe cechy, oraz "inteligencje". Dla przykladu, 
    jednym z rodziny zachowan, determinujacy pewnien typ "inteligentnych
    wskaznikow", moze automatyzowac destrukcje, oraz dezalokacje zasobu podczas
    niszczenia obiektu kontenera klasy DynamicMatrix
*/
template< typename T > 
class DynamicMatrix {
 public:
   typedef T               value_type;
   typedef T*              iterator;
   typedef const T*        const_iterator;
   typedef T&              reference;
   typedef const T&        const_reference;
   typedef std::size_t     size_type;
   typedef std::ptrdiff_t  difference_type;

   /** 
     @brief 
       Konstruktor rezerwujacy obszar pamieci potrzebnej do przechowania
       <i>width * height</i> elementow typu value_type

     @param
       width Wielkosc okreslajaca ilosc kolumn macierzy

     @param 
       height Wielkosc okreslajaca ilosc wierszy macierzy

     @remark 
       Wartosci rowne zeru, przekazane w formie argumentow <i>width</i>, 
       oraz <i>height</i> powoduja niezdefiniowane zachowanie!<br>
       
       Podczas konstrukcji obiektu typu DynamicMatrix dla kazdego 
       elementu przechowywanego w macierzy, zostaje wywolany domyslny 
       konstruktor skojarzony z typem value_type. Zachowanie to jest 
       implikowane przez wywolanie globalnego operatora dynamicznej
       alokacji pamieci - operator new[]. W rezultacie agregegowany
       typ jest zmuszony posiadac publiczny konstruktor domniemany.
       Dla obiektow bedacych typami podstawowymi (int, float, itd.)
       poczatkowe wartosci sa niezdefiniowane!

     @exception
       std::bad_alloc
   */
   DynamicMatrix( int  height, 
                  int  width );

   /** 
     @brief 
       Konstruktor z dodatkowym argumentem, przydzielajacy wymagany
       obszar pamieci wielkosci width * height * sizeof( value_type ), 
       oraz wywolujacy dla kazdego agregatu konstruktor kopiujacy,
       z parametrem w postacji wartosci initValue

     @param 
       width Wielkosc okreslajaca ilosc kolumn macierzy

     @param 
       height Wielkosc okreslajaca ilosc wierszy macierzy

     @param 
       initValue Wartosc obiektu jest wykorzystywana podczas wywolan
       konstruktora kopiujacego, kolejno dla kazdego tworzonego 
       elementu agregatu
     
     @remark 
       Wydedukowany parametr typu TypeValue nie musi posiadac
       konstruktora domyslnego, w sytuacji gdy tworzona instancja
       klasy szablonowej DynamicMatrix korzysta z omawianego konstruktora
   */
   DynamicMatrix( int              height, 
                  int              width, 
                  const_reference  initValue );
   
   /** 
     @brief 
       Zwykly konstruktor kopiujacy propagujacy semantyke kopiowania wartosci

     @remark
       W przypadku wystapienia inicjalizacji obiektu, instancja dokladnie
       tego samego wydedukowanego typu, zostaje wywolany zwykly konstruktor
       kopiujacy klasy. Brak jego definicji (istnienie jedynie metody szablonowej
       konstruktora kopiujacego), powodowalby wygenerowanie przez kompilator
       domyslnego konstruktora kopiujacego, ktorego semantyka jest niepoprawna
       (kopiowanie bit po bicie, a nie skladnik po skladniku).       
   */ 
   DynamicMatrix( const DynamicMatrix& rhs );

   /** 
     @brief 
       Zwykly wzorzec konstruktora kopiujacego

     @remark 
       Tak jak w przypadku konstruktora z jawnym argumentem
       inicjalizacyjnym, dzieki wykorzystaniu metody przydzialu pamieci zwanej 
       "umieszczajacym operatorem new" realizacja konstruktora nie zawiera 
       redundantnych wywolan konstruktorow domyslnych, wystepujacych w czasie 
       "standardowego" dynamicznego przydzielania pamieci na elementy 
       macierzy.<br>
       Wykorzystanie szablonu do utworzenia rodziny konstruktorow kopiujacych
       ma na celu umozliwienie inicjalizacji hipotetycznej macierzy, dla ktorej
       parametr T jest dedukowany typem wskaznikowym na klase podstawowa, 
       referencja macierzy parametryzowanej typem wskaznikowym klasy 
       pochodnej.<br>
       Byc moze klarowniejszym wytlumaczeniem powyzszego sformulowania 
       bedzie sprowadzenie problemu na nizszy poziom:<br>
       Wzorzec umozliwia przypisanie elementowi BASE* wartosci typu DERIVED*
   */
   template< typename U >
     DynamicMatrix( const DynamicMatrix< U >& rhs );
   
   /** 
     @brief 
       Zadaniem destruktora jest poprawne zwolnienie 
       przydzielonej pamieci, ktore mialo miejsce podczas 
       tworzenia obiektu (lub podczas operacji przypisania), 
       w zaleznosci od metody alokacji:
       - z uzyciem wywolania operatora new
       - z uzyciem metody umieszczajacego operatora new
   */
   ~DynamicMatrix();

   /** 
     @brief 
       Zwykly operator przypisania

     @remark 
       Definicja operatora przypisania wystepuje w definicji klasy
       z tych samych powodow, z jakich wynikla potrzeba istnienia 
       konstruktora kopiujacego. Metody szablonowe nie konkretyzuja operatora
       przypisania dla tych samych typow
   */
   DynamicMatrix& operator =( const DynamicMatrix& rhs );

   /** 
     @brief 
       Operator umozliwiajacy przypisanie macierzy roznych rozmiarow
       (z uwzglednieniem tozsamosci parametryzowanej nazwy typu T)
     
     @remark 
       Dodatkowa mozliwoscia (jak i w przypadku konstruktora kopiujacego)
       jest mozliwosc przypisania macierzy typu konkretnego [ T = BASE* ],
       macierzy typu [ T = DERIVED* ]

     @see 
       DynamicMatrix( const DynamicMatrix< U >& that )<br>
       ~DynamicMatrix()
   */
   template< typename U >
     DynamicMatrix& operator =( const DynamicMatrix< U >& rhs );

   /**
     @brief
       Operacja podmiany wewnetrznych wartosci wykorzystywanych w prywatnej
       implementacji klasy (ekwiwalent zachowania wykorzystywanego w STL,
       w realizacjach kontenerow - <i>container</i>::swap(<i>container</i>))
   */
   DynamicMatrix& swap( DynamicMatrix&  matrix );
   
   /** 
     @brief 
       Metoda akcesorowa dajaca dostep do elementu o zadanym indeksie,
       okreslajacym numer wiersza macierzy

     @param 
       index Wartosc okreslajaca odwolanie do elementu znajdujacego
       sie w wierszu <i>index</i> macierzy

     @return 
       Adres poczatku pamieci, od ktorego rozpoczyna sie bufor
       dla wiersza o wartosci index. Poprzez powtorne wyluskanie
       zwroconego przez metode wskaznika, z pomoca operatora odwolania 
       sie do elementu tablicy (operator[]), mozemy uzyskac dostep do 
       wartosci zapisanej w macierzy, korzystajac z intuicyjnego 
       zapisu: <b>macierz[wiersz][kolumna]</b>
     
     @remark 
       Kazda proba odwolania sie do elementu spoza wyznaczonego zakresu
       okreslonego poprzez liczbe wierszy (wartosc zwrocona przez metode width())
       wywola blad asercji.<br>
       Zatem, oczywista informacja jest, iz zakres wartosci kolumn nie jest 
       sprawdzany poprzez asercje (drugie posluzenie sie operatorem [])
   */
   T* operator []( std::size_t index );
   
   /** 
     @see operator []( std::size_t index )
   */
   const T* operator []( std::size_t index ) const;

   /** 
     @brief 
       Wartosc okreslajaca ilosc kolumn danej macierzy
   */
   int width() const;
   
   /** 
     @brief 
       Wartosc okreslajaca ilosc wierszy danej macierzy
   */
   int height() const;

   iterator begin() { return elements_; }
   iterator end()   { return elements_ + size(); }

   const_iterator begin() const { return elements_; }
   const_iterator end()   const { return elements_ + size(); }

   size_type size() const { return width() * height(); }

 private:
   T* memAllocWithDefaultCtors( int elementsCount ); 
   T* memAllocWithoutCtors( int elementsCount );

   template< typename U > void 
     init( const DynamicMatrix< U >& rhs );
   template< typename U > DynamicMatrix&
     assign( const DynamicMatrix< U >& rhs );

 private:
   int   width_;    /**< Liczba kolumn macierzy (jej szerokosc) */
   int   height_;   /**< Liczba wierszy macierzy (jej wysokosc) */
   T*    elements_; /**< Adres poczatku danych macierzy */
   bool  allocatedWithCtors_; /**< Flaga informujaca o specyficznej formie 
                                   przydzialu pamieci */
};


//
template< typename T > DynamicMatrix< T >
::DynamicMatrix( int  height, 
                 int  width ) 
  : elements_( memAllocWithDefaultCtors( width * height ) )
  , width_( width )
  , height_( height )
{
}

//
template< typename T > DynamicMatrix< T >
::DynamicMatrix( int              height, 
                 int              width, 
                 const_reference  initValue )
  : elements_( memAllocWithoutCtors( width * height ) )
  , width_( width )
  , height_( height ) 
{
  const int  ELEMENT_COUNT = height * width;
  
  for( int  elementOffset = 0; 
    elementOffset < ELEMENT_COUNT; 
    ++elementOffset )
  {
    new( elements_ + elementOffset ) value_type( initValue );
  }
}

//
template< typename T > template< typename U > DynamicMatrix< T >
::DynamicMatrix( const DynamicMatrix< U >& rhs )
{
  init( rhs );
}

//
template< typename T > DynamicMatrix< T >
::DynamicMatrix( const DynamicMatrix& rhs ) 
{
  init( rhs );
}

//
template< typename T > DynamicMatrix< T >
::~DynamicMatrix() 
{
  if( allocatedWithCtors_ ) 
  {
    const int  ELEMENT_COUNT = width() * height();
    const int  LAST_ELEMENT  = ELEMENT_COUNT - 1;
    const int  FIRST_ELEMENT = 0;
    
    for( int  reverse = LAST_ELEMENT; 
      reverse >= FIRST_ELEMENT; 
      --reverse )
    {
      value_type*  destroyedInstance = elements_ + reverse;
      destroyedInstance->T::~T();
    }
    
    ::operator delete( elements_ );
  } 
  else
    delete [] elements_;
  
  elements_ = 0;
}

//
template< typename T > DynamicMatrix< T >& DynamicMatrix< T >
::operator =( const DynamicMatrix& rhs ) 
{
  return assign( rhs );
}

//
template< typename T > 
template< typename U > DynamicMatrix< T >& DynamicMatrix< T >
::operator =( const DynamicMatrix< U >& rhs ) 
{
  return assign( rhs );
}

//
template< typename T > DynamicMatrix< T >& DynamicMatrix< T >
::swap( DynamicMatrix&  matrix )
{
  std::swap( width_, matrix.width_ );
  std::swap( height_, matrix.height_ );
  std::swap( elements_, matrix.elements_ );
  std::swap( allocatedWithCtors_, matrix.allocatedWithCtors_ );
  
  return *this;
}

//
template< typename T > T* DynamicMatrix< T >
::operator []( std::size_t  index )
{
  assertion( index < height(),
    "Indeks spoza dozwolonego zakresu!" );

  return elements_ + index * width();
}

//
template< typename T > const T* DynamicMatrix< T >
::operator []( std::size_t index ) const
{
  assertion( index < height(),
    "Indeks spoza dozwolonego zakresu!" );

  return elements_ + index * width();
}

//
template< typename T > inline int DynamicMatrix< T >
::width() const 
{ 
  return width_;
}

//
template< typename T > inline int DynamicMatrix< T >
::height() const
{ 
  return height_;
}

//
template< typename T > T* DynamicMatrix< T >
::memAllocWithDefaultCtors( int  elementsCount ) 
{ 
  assertion( elementsCount > 0,
    "Nie mozna zarezerwowac pamieci o zerowej, lub mniejszej wielkosci" );
  
  allocatedWithCtors_ = false;
  
  return new T[ elementsCount ];
}

//
template< typename T > T* DynamicMatrix< T >
::memAllocWithoutCtors( int  elementsCount ) 
{
  assertion( elementsCount > 0,
    "Nie mozna zarezerwowac pamieci o zerowej, lub mniejszej wielkosci" );

  allocatedWithCtors_ = true;

  return static_cast< T* >( ::operator new( elementsCount * sizeof( T ) ) );
}

//
template< typename T > template< typename U > void DynamicMatrix< T >
::init( const DynamicMatrix< U >& rhs )
{
  elements_ = memAllocWithoutCtors( rhs.width() * rhs.height() );
  width_    = rhs.width();
  height_   = rhs.height();

  for( int hgh = 0; hgh < height(); ++hgh ) {
    for( int wdh = 0; wdh < width(); ++wdh ) 
    {
      T* iterElem = ( *this )[ hgh ] + wdh;
      new( iterElem ) T( static_cast< const T& >( rhs[ hgh ][ wdh ] ) );
    }
  }
} 

//
template< typename T > template< typename U > 
DynamicMatrix< T >& DynamicMatrix< T >
::assign( const DynamicMatrix< U >& rhs )
{
  DynamicMatrix< T >  temp( rhs );
  return swap( temp );
}

} // namespace Utilities
} // namespace Gcad

#endif

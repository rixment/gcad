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

#ifndef _GCAD_REFCOUNTPTR_H_
#define _GCAD_REFCOUNTPTR_H_

#include "GcadAssertion.h"
#include <map>

namespace Gcad {
namespace Utilities {

/**
  @brief
    Klasa "inteligentnych wskaznikow"
    
    Klasa "inteligentnych wskaznikow" wykorzystujaca techniki:
    - zliczania odniesien
    - pozyskanie zasobow jest inicjalizacja
    
    <b>Instancje konkretynych klas szablonowych w postaci danych 
    skladowych klas nie podlegaja kopiowaniu zgodnego
    z semantyka parametryzowanego typu instancji klasy.</b><br>
    Gdy takie zachowanie nie jest pozadane, nalezy rozpatrzyc mozliwosc
    uzycia szablonu low::ValuePtr, ktory adaptuje zautomatyzowana semantyke
    metod kopiujacych typu operator przypisania, oraz konstruktor kopiujacy.
    Wiecej informacji dotyczacych poprawnego dzialania szablonow inteligentnych
    wskaznikow mozna znalezc w publikacji "More Exceptional C++" Herb'a Sutter'a
    (zagadnienie 30, strona 186). Ogolne zalety implikujace uzycie
    obiektowych odpowiednikow wskaznikowych encji rowniez opisane sa 
    w "More Effective C++" Scott'a Meyers'a.

  @remark 
    Nalezy zauwazyc brak metod szablonowych rodzaju: <i>operatora 
    przypisania</i>, oraz <i>konstruktora kopiujacego</i>, ktore
    umozliwialyby przypisywanie inteligentnych wskaznikow obiektow klas 
    pochodnych, inteligentnch wskaznikow klas podstawowych (standardowy 
    szablon auto_ptr umozliwia takie kopiowania). Pominiecie tych skladowych 
    nie jest bezcelowe. Obiekty tej klasy nie reprezentuja jedynie "wskaznika 
    zarzadzajacego zasobami", wykorzystywanego w technice "pozyskiwanie zasobow 
    jest inicjalizacja", lecz rowniez wykorzystuja metode zliczania odniesien.
    Poniewaz w klasie znajduje sie zmienna statyczna implementujaca mechanizm
    zliczania odniesien, umozliwienie takich przypisan, powodowaloby problemy
    zwiazane z inkrementacja (wzglednie dekrementacja) licznika odniesien,
    ktory "rozsiany" bylby pomiedzy roznymi danymi statycznymi skonkretyzowanych
    klas. Ta "niedogodnosc" mozna w prosty sposob ominac, poslugujac sie jawnym 
    rzutowaniem wskaznika na typ podstawowy podczas przypisania, lub 
    inicjalizacji.<br>
    Pozostawienie konstruktora bez modyfikatora explicit, moze nie jest 
    najlepszym rozwiazaniem, ale dzieki takiemu postepowaniu, skladnia przypisan,
    oraz inicjalizacji jest mniej skomplikowana (nie trzeba poslugiwac sie jawnym
    rzutowaniem do typu "inteligentnego wskaznika", poniewaz kompilator widzac
    adres typu pochodnego, ktory jest przypisywany (ewentualnie wystepuje w roli
    inicjalizatora) do "inteligentnego wskaznika" typu bazowego, podda go 
    konwersji na typ bazowy, po czym wykona niejawne wywolanie konstruktora, tym
    samym tworzac nowy obiekt-wskaznik, ktory moze byc uzyty w postaci wartosci
    inicjalizujacej)<br>
    Wazna kwestia jest rowniez fakt, iz wskazniki przekazywane do obiektow
    typu RefCountPtr powinny wskazywac na obiekty utworzone w stercie za pomoca
    funkcji operatora new. Przekazujac adresy obiektow  lokalnych, statycznych, 
    globalnych, a nawet tablic obiektow utworzonych dynamicznie za posrednictwem
    wywolania operatora new[], powodowac beda zalamanie programu podczas
    wykonywania destruktora, ktorego cialo zawiera instrukcje operatora 
    delete<br>
  
  @param
    T Typ instancji bedacej pod kontrola obiektu RefCountPtr< T >

  @code
    #include "GcadRefCountPtr.h"
    #include <iostream>

    using namespace Gcad::Utilities;
    using namespace std;

    void main() 
    {
      RefCountPtr< int >  autoReleaseMemExternScope;
      
      {
        RefCountPtr< int >  autoReleaseMemInternalScope( new int( 0xBOB ) );
        
        autoReleaseMemExternScope = autoReleaseMemInternalScope;
      }
      
      cout << "Extern scope object dereference: " << 
        *autoReleaseMemExternScope << endl;

      // Konczy sie zasieg obiektu autoReleaseMemExternScope,
      // tym samym nastepuje automatyczna dezalokacja pamieci
    }
  @endcode
*/
template< typename T > class RefCountPtr {
 public:
   /** 
     @brief 
       Konstruktor wykonuje prace, polegajaca na zwiekszeniu 
       licznika odniesien
     
     @param 
       pointer Wskaznik obiektu utworzonego w stercie
   */
   RefCountPtr( T* pointer = 0 );
   
   /** 
     @brief 
       Konstruktor kopiujacy zwieksza licznik odniesien danego obiektu
   */
   RefCountPtr( const RefCountPtr& that );

   /**
     @brief 
       Destruktor dekrementuje licznik odniesien. Gdy wyniesie 
       on zero, wowczas obiekt instancji RefCountPtr jest odpowiedzialny
       za automatyczne zniszczenie, oraz dezalokacje pamieci kontrolowanej
       instancji
   */
   ~RefCountPtr();

   /** 
     @brief 
       Operator wykonuje zarowno dekrementacje (obiekt bedacy celem 
       przypisania), oraz inkrementacje (inicjalizator) licznika odwolan
   */
   RefCountPtr& operator =( const RefCountPtr& rhs );

   /** 
     @brief 
       Operator przypisania dla obiektow przydzielanych dynamicznie
   */
   RefCountPtr& operator =( T* rhs );

   /** 
     @brief 
       Wyluskanie obiektu, ktory dostepny jest posrednio poprzez wskaznik
   */
   T& operator *() const;
   
   /** 
     @brief 
       Odwolanie sie do skladnikow wewnetrznej struktury 
       obiektu podlegajacego zliczaniu odniesien
   */
   T* operator ->() const;

   /** 
     @brief 
       Pozyskanie adresu obiektu, ktory podlega zliczaniu odniesien
     
     @remark
       Korzystanie ze zwroconej wartosci moze przyczynic sie do
       powstania trudno uchwytnych bledow. Dzieje sie tak dlatego, iz
       w przypadku osiagniecia wartosci zero przez licznik odniesien
       danej instancji, obiekt "inteligentnego wskaznika" dezalokuje
       obszar pamieci kontrolowanej przez niego instancji
   */
   T* get() const;

   /**
     @brief
       Przekazanie odpowiedzialnosci za zniszczenie obiektu klientowi
       wywolujacego metode

     @remark
       W przypadku proby przekazania odpowiedzialnosci wskaznika pustego
       zostanie zgloszony blad asercji.<br>
       Po przeslaniu komunikatu do instancji inteligentnego wskaznika,
       odpowiedzialnosc za usuniecie obiektu (przydzielonego na stercie)
       spoczywa na kliencie-nadawcy
   */
   T* release() const;

 private:
   /** 
     @brief 
       Adres instancji obiektu zaalokowanego w stercie
   */
   mutable T*  ptr_;

   /** 
     @brief 
       Zwraca kontener przechowujacy adresy obiektow (sprzezonych z
       licznikami odniesien) dla danego typu T (konkretyzacji szablonu)
    
     @remark 
       Poniewaz funkcja wykorzystuje zmienna statyczna, dlatego nie 
       zostala zadeklarowana, jako inline. Najprawdopodobniej kompilator 
       powinien poradzic sobie z taka konstrukcja (jesli jest zgodny 
       z postanowieniami "standardu"), jednak ze wzgledow bezpieczenstwa 
       jest pozbawiona modyfikatora inline
   */
   static std::map< T*, int >& refMap();
};


//
template< typename T > 
std::map< T*, int >& 
RefCountPtr< T >
::refMap()
{
  // Wykorzystanie wzorca Singleton gwarantujacego inicjalizacje 
  // przed pierwszym uzyciem
  static std::map< T*, int > refPtrCounter;
  return refPtrCounter;
}

//
template< typename T > 
bool
operator ==( const RefCountPtr< T >&  lhs,
             const RefCountPtr< T >&  rhs )
{
  return lhs.get() == rhs.get();
}

//
template< typename T > 
bool
operator !=( const RefCountPtr< T >&  lhs,
             const RefCountPtr< T >&  rhs )
{
  return !( lhs == rhs );
}

// 
template< typename T > 
bool
operator <( const RefCountPtr< T >&  lhs,
            const RefCountPtr< T >&  rhs )
{
  return lhs.get() < rhs.get();
}

//
template< typename T > 
RefCountPtr< T >
::RefCountPtr( T*  pointer )
  : ptr_( pointer )
{
  // Zakladajac, iz przekazany adres ma poprawna wartosc, nalezy
  // zwiekszyc licznik odniesien obiektu wskazywanego przez ten wskaznik
  // Jesli przekazany wskaznik jest rozny od wartosci zerowej, wowczas 
  // sprawdz, czy odpowiadajaca mu wartosc nie znajduje sie juz w kontenerze.
  // Nastepnie, w zaleznosci od powyzszego stwierdzenia, ustal odpowiadajaca
  // "wspolnemu zasobowi" wartosc licznika odniesien
  if( ptr_ ) {
    if( refMap().count( ptr_ ) == 0 )
      refMap()[ ptr_ ] = 1;
    else
      ++refMap()[ ptr_ ];
  }
}

//
template< typename T > 
RefCountPtr< T >
::RefCountPtr( const RefCountPtr&  that )
{
  new( this ) RefCountPtr( that.ptr_ );
}

//
template< typename T > 
RefCountPtr< T >
::~RefCountPtr()
{
  // Odnajdz obiekt reprezentujacy sprzezona wartosc wskaznik-licznik odniesien.
  // Jezeli obiekt nie zostanie odnaleziony, to nie wykonuj zadnej czynnosci
  // (malo prawdopodobne), w przeciwnym wypadku zmiejsz jego licznik odwolan.
  // W przypadku, gdy licznik odniesien wynosi zero, wowczas nalezy zwolnic
  // nieuzywany zasob (powinien on znajdowac sie w stercie)
  typename std::map< T*, int >::iterator found = refMap().find( ptr_ );
  
  if( found != refMap().end() && 
    --found->second == 0 )
  {
    delete ptr_;
    ptr_ = 0;
  }
}

//
template< typename T > 
RefCountPtr< T >& 
RefCountPtr< T >
::operator =( const RefCountPtr& rhs )
{
  // Jesli cel przypisania nie jest nowo utworzonym "wskaznikiem", oraz
  // Jezeli obiekty "wskaznikow" odwoluja sie do tego samego obiektu, to nie
  // wykonuj zadnych dzialan
  if( ptr_ != 0 && *this == rhs )
    return *this;
  
  // 1. Zmniejsz wartosc odwolan dla obiektu, do ktorego aktualnie odnosi sie
  // inteligentny wskaznik. W wypadku, gdy zaden z obiektow wskaznikow nie
  // odwoluje sie do wspolnej wartosci, wowczas zwolnij zasob.
  // 2. Ustaw odwolanie do nowego zasobu, oraz zwieksz jego licznik odwolan
  this->RefCountPtr< T >::~RefCountPtr();
  new( this ) RefCountPtr( rhs );
  return *this;
}

//
template< typename T > 
RefCountPtr< T >& 
RefCountPtr< T >
::operator =( T* rhs )
{
  if( ptr_ != 0 && ptr_ == rhs )
    return *this;

  this->RefCountPtr< T >::~RefCountPtr();
  new( this ) RefCountPtr( rhs );
  return *this;
}

//
template< typename T > 
T& 
RefCountPtr< T >
::operator *() const
{
  assertion( ptr_ != 0, 
    "Proba wyluskania pustego wskaznika" );
  return *ptr_;
}

//
template< typename T > 
T*
RefCountPtr< T > 
::operator ->() const
{
  assertion( ptr_ != 0, 
    "Proba wyluskania pustego wskaznika" );
  return ptr_;
}

//
template< typename T > 
T*
RefCountPtr< T > 
::get() const 
{
  return ptr_;
}

//
template< typename T > 
T* 
RefCountPtr< T > 
::release() const
{
  assertion( ptr_ != 0, 
    "Proba zwolnienia pustego wskaznika" );

  T* ptrToRelease = ptr_;
  ptr_ = 0;
  return ptrToRelease;
}

} // namespace Utilities
} // namespace Gcad

#endif
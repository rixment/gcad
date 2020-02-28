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

#ifndef _GCAD_MEMORYALLOC_H_
#define _GCAD_MEMORYALLOC_H_

#include "GcadAssertion.h"
#include <memory>  // std::bad_alloc
#include <utility> // std::pair

namespace Gcad {
namespace Utilities {

/** 
  @brief
    Szablon definiuje funkcje dynamicznego przydzialu pamieci
    
    Szablon definiuje funkcje dynamicznego przydzialu, oraz dezalokacji 
    pamieci dla obiektow typu pochodnego, ktory wykorzystuje implementacje
    skonkretyzowanej klasy szablonowej, utworzonej na bazie omawianego 
    szablonu. Godnym uwagi jest fakt, iz omawiana implementacja menadzera
    pamieci, zeby byc scislym - konkretna klasa bazowa, potrafi obsluzyc 
    obiekty roznych klas pochodnych, tym samym implikujac poprawne 
    przeprowadzanie dzialan dla instancji odmiennych rozmiarow.<br>
    Struktura bufora jest reprezentowana w postaci listy segmentow, 
    ktora zakonczona jest wezlem pustym. Taka konstrukcja umozliwia
    dynamiczne organizowanie, oraz kontrole obszaru wolnej pamieci
  
  @param 
    ELEM_COUNT Wartosc okreslajaca maksymalna ilosc przechowywanych 
    elementow w zarezerwowanym buforze pamieci (ang. memory pool)

  @param 
    SEG_SIZE Rozmiar podstawowego budulca, z ktorego skonstruowany jest
    bufor - wartosc wyrazona w bajtach przypadajaca na segment. Optymalne
    preferowane wartosci w przypadku architektur 32 bitowych, powinny 
    byc ich wielokrotnosciami
  
  @remark
    Przed pierwszym uzyciem rodziny metod operator new, nalezy wykonac
    inicjalizacje obszaru za posrednictwem metody purgeMemory()

  @code
    #include "GcadMemoryAlloc.h"
    #include <iostream>
    #include <memory>

    using namespace Gcad::Utilities;
    using namespace std;

    const int ELEMENTS_NUM = 100;
    const int SEGMENT_SIZE = sizeof( int ) * 5;

    typedef MemoryAlloc< ELEMENTS_NUM, SEGMENT_SIZE >  UserDefinedMemAlloc;

    class Simple : public UserDefinedMemAlloc {
     public:
       friend std::ostream& operator <<( std::ostream& out,
                                         const Simple& simple )
       {
         for( int* itor = fiveInts_;
           itor != fiveInts_ + 5;
           ++itor )
         {
           out << *itor << ", ";
         }
         return out;
       }
     
     private:
       int  fiveInts_[ 5 ];
    };

    int main()
    {
      UserDefinedMemAlloc::purgeMemory();

      // Dziala MemoryAlloc< ELEMENTS_NUM, SEGMENT_SIZE >::operator new
      std::auto_ptr< Simple > instance( new Simple ); 
      
      // Dziala MemoryAlloc< ELEMENTS_NUM, SEGMENT_SIZE >::operator new[]
      Simple* array = new Simple[ 10 ];

      const int ARRAY_INDEX_MAGIC = 3;
      cout << *instance << endl << array[ ARRAY_INDEX_MAGIC ] << endl;

      delete [] array;      
    }
  @endcode

  @todo
    Umiescic tablice w funkcjach jako statyczne dane, tym samym zapewniajac
    inicjalizacje przed pierszym uzyciem
*/
template< int ELEM_COUNT, 
          int SEG_SIZE = sizeof( int ) >
class MemoryAlloc {
 public:
   /** 
     @brief 
       Zasloniecie domyslnej funkcjonalnosci globalnego operatora new
       w klasach pochodnych. Dla instancji podtypow, w czasie dynamicznej 
       alokacji pamieci bedzie wykorzystywana ta wersja operatora

     @exception
       std::bad_alloc Obiekt rzucony w przypadku braku wystarczajacej ilosci
       pamieci do wykonania uslugi jej udostepnienia
   */
   static void* operator new( size_t bytesCount ) throw( std::bad_alloc );
  
   /** 
     @brief 
       Zasloniecie (w klasach pochodnych) globalnego operatora new[]
     
     @see 
       void* operator new( size_t )

     @exception
       std::bad_alloc
   */
   static void* operator new []( size_t bytesCount ) throw( std::bad_alloc );
   
   /** 
     @brief 
       Zasloniecie globalnej funkcji operatora delete.
       Klasy pochodne podczas zwalniania pamieci przydzielonej 
       "na stercie" beda uzywac tej pzeslonietej wersji operatora delete
   */
   static void operator delete( void* memoryToRelease ) throw();
   
   /** 
     @brief 
       Zasloniecie globalnej funkcji operatora delete[]
     
     @see void operator delete( void* )
   */
   static void operator delete []( void* memoryToRelease ) throw();
   
   /** 
     @brief 
       Przywrocenie stanu surowego pamieci (wykonanie dezalokacji
       wszystkich zarezerwowanych segmentow)
   */
   static void purgeMemory();

   /** 
     @brief 
       Podany w bajtach rozmiar calego bloku pamieci 
   */
   static int size();
       
   /** 
     @brief 
       Ilosc bajtow przypadajaca na segment 
   */
   static int bytesPerSegment();
       
   /** 
     @brief 
       Liczba segmentow, z ktorych sklada sie caly obszar dostepnej pamieci 
   */
   static int segmentsCount();

 protected:
   /** 
     @brief 
       Domyslny konstruktor, wywolywany podczas tworzenia
       instancji klas pochodnych

     @remark 
       Cialo konstruktora nie zawiera instrukcji, poniewaz
       klasa udostepnia jedynie statyczne dane, oraz pracujace
       na nich funkcje, wiec instancje tej klasy nie reprezentuja
       zadnego pojecia ze swiata rzeczywistego
   */
   MemoryAlloc() {}

 private:
   /** 
     @brief 
       Opisowa nazwa uzywana przez funkcje findFreeMemSpace( size_t )

     @remark 
       Synonim pary, dla ktorej pierwsza wartosc determinuje
       indeks poczatku odnalezionego obszaru, zdolnego pomiescic 
       zadana przez uzytkownika (wywolanie operatora new) ilosc danych. 
       Druga z wartosci okresla indeks obszaru poprzedzajacego 
       dany segment (jest ona potrzebna do prawidlowego sprzezenia wezlow)
   */
   typedef std::pair< int, int >  FoundedAndPrevBlocks;

   /** 
     @brief 
       Wykonanie operacji specyficznych dla wewnetrznej
       struktury, oraz oddanie obszaru pamieci do uzytku 
       rozpoczynajacego sie od wartosci zwroconego adresu
   */
   static void* giveBlock( size_t size ) throw( std::bad_alloc );
   
   /** 
     @brief 
       Zadaniem metody jest odnalezienie obszaru pamieci, 
       ktory jest w stanie pomiescic obiekt o wielkosci 
       wyrazonej przez formalny argument blockCount
   */
   static FoundedAndPrevBlocks 
     findFreeMemSpace( size_t blockCount ) throw( std::bad_alloc ); 

   /** 
     @brief 
       Struktura organizujaca dane, wykorzystywana w wewnetrznej 
       implementacji menadzera pamieci

     @remark 
       Kazdy wezel, z pominieciem ostatniego, posiada zawsze wartosc 
       rozna od zera. Na tej podstawie okreslany jest koniec listy (wolnej
       pamieci) podczas trawersacji przez wezly
   */
   struct InfoNode {
     /** 
       @brief 
         Sprawdzenie, czy wezel jest koncem listy
     */
     bool isEnd() { return seg_ == 0; }

     /** 
       @brief 
         Wartosc okreslajaca kolejny wezel listy

       @remark 
         Laczniki sa zaimplementowane w postaci tablicy 
         liczb calkowitych, wiec dla scislosci mozna dodac, iz
         wartosc determinuje indeks tablicy menadzera pamieci
         (przesuniecie wzgledem jej poczatku)
     */
     int  next_;

     /** 
       @brief 
         Ilosc aktualnie zwolnionych segmentow (na rzecz 
         wywolan rodziny funkcji operator new) nastepujacych 
         po biezacym wezle

       @remark 
         Wartosc bedzie wieksza od jednosci dla obiektow tablic, 
         oraz dla instancji o liczbie bajtow przekraczajacych rozmiar 
         segmentu (bytesPerSegment())
     */
     int  seg_; 
   };

   /** 
     @brief 
       Wartosc determinujaca poczatek wolnej pamieci 
       (indeks aktualngo umiejscowienia glowy listy) 
   */
   static int  head_;
   
   /**
     @brief 
       Tablica lacznikow wezlow segmentow listy. +1 dodatkowy wezel pusty
   */
   static InfoNode  links_[ ELEM_COUNT + 1 ];
   
   /** 
     @brief 
       Obszar wolnej pamieci przeznaczonej do wykorzystania 
       przez obiekty klas pochodnych

     @remark 
       Standard zapewnia, iz dla wszystkich implementacji C++ typu
       char jest rowny jednemu bajtowi, stad wielkosc obszaru 
       wolnej pamieci, po skonkretyzowaniu szablonu wynosi 
       ELEM_COUNT * SEG_SIZE bajtow
   */
   static char  memoryPool_[ ELEM_COUNT * SEG_SIZE ];
 
 private:
   // nie zaimplementowane
   MemoryAlloc( const MemoryAlloc& );
   MemoryAlloc& operator =( const MemoryAlloc& );
};


//
template< int ELEM_COUNT, int SEG_SIZE > 
int MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::head_ = 0;

//
template< int ELEM_COUNT, int SEG_SIZE > 
typename MemoryAlloc< ELEM_COUNT, SEG_SIZE >::InfoNode
MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::links_[ ELEM_COUNT + 1 ] = { 0 };

//
template< int ELEM_COUNT, int SEG_SIZE > 
char MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::memoryPool_[ ELEM_COUNT * SEG_SIZE ] = { 0 };

//
template< int ELEM_COUNT, int SEG_SIZE > inline 
void* MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::operator new []( size_t bytesCount ) throw( std::bad_alloc )
{
  return operator new( byteSize );
}

//
template< int ELEM_COUNT, int SEG_SIZE > inline 
void MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::operator delete []( void* memoryToRelease ) throw() 
{
  delete static_cast< void* >( memoryToRelease );
}

//
template< int ELEM_COUNT, int SEG_SIZE > 
void MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::purgeMemory() 
{
  const int begLinksItor = 0;
  const int endLinksItor = ELEM_COUNT + 1;
  const int segmentIsAvaible = 1;

  for( int linksItor = begLinksItor;
    linksItor < endLinksItor; 
    ++linksItor ) 
  {
    links_[ linksItor ].next_ = linksItor + 1;
    links_[ linksItor ].seg_  = segmentIsAvaible;
  }

  head_ = begLinksItor;

  // Okreslenie konca listy - wezel pusty
  const int lastEmptyNode  = ELEM_COUNT;
  links_[ lastEmptyNode ].next_ = 0;
  links_[ lastEmptyNode ].seg_  = 0;
}

//
template< int ELEM_COUNT, int SEG_SIZE >
void MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::operator delete( void* memoryToRelease ) throw() 
{
  // Obliczamy ilosc elementow znajdujacych sie pomiedzy zakresem wyznaczonym
  // przez wskaznik p (konwertowany z void* na char*, aby mogla byc wykonana
  // arytmetyka wskaznikow), a wskaznikiem okreslajacym poczatek tablicy pamieci
  const int COUNT_BYTES_TO_RELEASED_MEM = 
    reinterpret_cast< char* >( memoryToRelease ) - memoryPool_;
  
  // Musimy podzielic otrzymany wynik przez wielkosc jednego bloku
  // pamieci, ktory odpowiada wartosci bytesPerSegment() bajtow. 
  // Rezultatem bedzie wartosc poprawnego indeku elementu o adresie 
  // memoryToRelease w pamieci. Dzieki temu bedziemy mogli zwrocic
  // odpowiadajacy blok (wzglednie bloki) do zasobu wolnej pamieci
  int memPoolIndex = COUNT_BYTES_TO_RELEASED_MEM / bytesPerSegment();

  // Jesli wezel nie pokazuje na samego siebie, jednoznacznie oznacza to, iz
  // probujemy usunac obszar pamieci, ktory zostal blednie okreslony w wywolaniu
  // funkcji operatorowej delete (memoryToRelease zawiera niepoprawny adres)
  if( links_[ memPoolIndex ].next_ != memPoolIndex )
    return;

  // Zapamietujemy nastepny lacznik, czyli indeks kolejnego wezla listy za
  // aktualnie wskazywanym przez glowe wolnego obszaru
  int next = links_[ head_ ].next_;

  // Ustawiamy w wezle, na ktory aktualnie pokazuje glowa listy obszaru
  // wolnego, indeks poczatku zwalnianego obszaru
  links_[ head_ ].next_ = memPoolIndex;
  
  // Podczas rezerwacji pamieci, gdy przydzielany jest obszar, lacznik
  // poczatkowego bloku jest ustawiany, aby wskazywal na samego siebie.
  // Jesli zwalniany obszar zajmuje jeden blok, wowczas lacznik trzeba ustawic,
  // aby wskazywal na wezel wskazywany przez glowe listy
  if( links_[ memPoolIndex ].seg_ == 1 ) {
    links_[ memPoolIndex ].next_ = next;
  } else {
    // W przypadku, gdy zwalniany obszar zajmuje kilka blokow, lacznik
    // pierwszego bloku trzeba ustawic na kolejnym, nastepujacym za nim bloku,
    // a ostatni z blokow zwalnianego obszaru, na wezel ktory byl poprzednio
    // wskazywany przez obiekt glowy wolnego obszaru, czyli na wezel o indeksie
    // ktory zostal wczesniej zapamietany w zmiennej next (wykonanie konkatencji
    // w celu uzyskania pozadanej struktury - listy)
    const int NEXT_NODE = 1;
    links_[ memPoolIndex ].next_ = memPoolIndex + NEXT_NODE;

    const int LAST_BLOCK = memPoolIndex + ( links_[ memPoolIndex ].seg_ - 1 );
    links_[ LAST_BLOCK ].next_ = next;
  }
}

//
template< int ELEM_COUNT, int SEG_SIZE > 
typename MemoryAlloc< ELEM_COUNT, SEG_SIZE >::FoundedAndPrevBlocks
MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::findFreeMemSpace( size_t blockCount ) throw( std::bad_alloc )
{
  // Zmienne okreslajace polozenie (indeks) wzgledem poczatku 
  // tablicy wolnej pamieci, kolejno: 
  //   - poczatku odnaleznionego bloku
  //   - bloku przed odnalezionym
  int  founded = head_;
  int  prev    = head_;

  // Przy zalozeniu, iz nalezy przydzielic pamiec dla wiecej niz jednego bloku,
  // petla wykona co najmniej blockCount-1 iteracji. Wskaznik search sluzacy do 
  // iteracji nie moze wskazywac na ostatni wezel
  size_t  continuousBlockCount = 0;
  while( links_[ founded ].isEnd() == false && 
    ++continuousBlockCount < blockCount ) 
  {
    // Szukamy ciaglego obszaru, ktory odpowiadalby ilosci blockCount blokow.
    // Jezeli wartosc lacznika sprawdzanego wezla nie "wskazuje" na nastepny 
    // wezel, wowczas mamy do czynienia z sytuacja, w ktorej aktualnie 
    // sprawdzany obszar nie jest w stanie sprostac wymogowi ciaglosci. W tym
    // wypadku, nalezy zaczac szukac od poczatku, pomijajac aktualny wezel
    if( links_[ founded ].next_ != founded + 1 ) {
      continuousBlockCount = 0;
      // zapisujemy indeks poprzedniego wezla, aby pozniej moc poprawnie
      // odlaczyc zwalniany obszar ( wezly ) z glownej listy
      prev = founded;
      // zapisujemy indeks kolejnego wezla znajdujacego sie za sprawdzonym
      // obszarem, aby od niego zaczynac kolejna iteracje znajdowania wolnej
      // przestrzeni
      founded = links_[ founded ].next_;
    } else {
      ++founded;
    }
  }
  
  // Jesli nie zostanie odnaleziony ciagly obszar w pamieci zdolny
  // zaspokoic wymagania stawiane przez klienta w postaci wartosci
  // okreslajacej wielkosc rezerwacji pamieci, wowczas nalezy oznajmic
  // zaistnienie sytuacji wyjatkowej
  if( links_[ founded ].isEnd() )
    throw std::bad_alloc();
  
  // Zwracamy wartosci przesuniec (indeksy), wzgledem poczatku puli pamieci.
  // Pierwsza wartosc okresla poczatek zwalnianego obszaru, natomiast druga
  // determinuje indeks obszaru poprzedzajacego. Jest ona wykorzystywana 
  // w celu poprawnej konkatencji wezlow listy
  return std::make_pair( founded - ( blockCount - 1 ), prev );
}

//
template< int ELEM_COUNT, int SEG_SIZE > 
void* MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::operator new( size_t bytesCount ) throw( std::bad_alloc ) 
{
  CommonUtil::assure( 
    bytesPerSegment() != 0, 
    "MemoryAlloc::operator new( size_t bytesCount ): Dedukowana wartosc "
    "okreslajaca wielkosc segmentu nie moze byc rowna zeru" );

  // Obliczenie prawidlowego rozmiaru okreslajacego ilosc 
  // przydzielanych segmentow, na podstawie wartosci zmiennej 
  // byteSize, ktora okresla rozmiar pamieci potrzebnej na
  // przechowanie obiektu. Rzutowanie do liczby zmiennopozycyjnej
  // implikuje rzutowanie zmiennej bytesPerSegment() i wykonania dzielenia
  // na liczbach zmiennoprzecikowych
  float neededSegments = static_cast< float >( bytesCount ) / bytesPerSegment();
  int floorNededSegments = static_cast< int >( neededSegments );

  // Rzadana wartosc okreslajaca wielkosc przydzialu 
  // (przydzielanych segmentow na dany obiekt)
  int blockCount = neededSegments != floorNededSegments ? 
    // Rozmiar pamieci rezerwowanej dla obiektu nie jest wyrownany
    // do rozmiaru segmentu, wiec trzeba przydzielic o jeden 
    // segment wiecej
    floorNededSegments + 1 : 
    // Sytuacja idealna. Rozmiar przydzielanego obszar 
    // jest dopasowany, rowny wielokrotnosci segmentu
    floorNededSegments;
  
  // Probujemy przydzielic obszar
  return giveBlock( blockCount );
}

//
template< int ELEM_COUNT, int SEG_SIZE > 
void* MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::giveBlock( size_t blockCount ) throw( std::bad_alloc )
{
  FoundedAndPrevBlocks  foundedAndPrevBlocks = findFreeMemSpace( blockCount );
  const int foundedBlock   = foundedAndPrevBlocks.first;
  const int previousBlock  = foundedAndPrevBlocks.second;

  // Zostal odnaleziony obszar pamieci i znajduje sie on
  // dokladnie w miejscu obecnie wskazywanym przez glowe listy
  if( previousBlock == head_ ) {
    head_ = links_[ foundedBlock + blockCount - 1 ].next_;
  }
  // Zostal odnaleziony obszar, ktory ma indeks rozny od 
  // aktualnie zapisanego w glowie listy wolnej pamieci
  else {
    links_[ previousBlock ].next_ = 
      links_[ foundedBlock + blockCount - 1 ].next_;
  }
  
  // Ustawiamy indeks lacznika na wartosc rowna wlasnemu wezlowi.
  // Zaznaczamy tym samym, iz obszar jest zarezerwowany
  links_[ foundedBlock ].next_ = foundedBlock;
  links_[ foundedBlock ].seg_  = blockCount;

  // zwracamy adres przydzielonego obszaru
  return memoryPool_ + foundedBlock * bytesPerSegment();
}

//
template< int ELEM_COUNT, int SEG_SIZE > inline 
int MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::size() const
{
  return ELEM_COUNT * SEG_SIZE;
}
       
//
template< int ELEM_COUNT, int SEG_SIZE > inline 
int MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::bytesPerSegment() const
{
  return SEG_SIZE;
}
       
//
template< int ELEM_COUNT, int SEG_SIZE > inline
int MemoryAlloc< ELEM_COUNT, SEG_SIZE >
::segmentsCount() const
{
  return ELEM_COUNT;
}

} // namespace Utilities
} // namespace Gcad

#endif
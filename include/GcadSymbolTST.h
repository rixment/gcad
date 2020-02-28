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

#ifndef _GCAD_SYMBOLTST_H_
#define _GCAD_SYMBOLTST_H_

#include "GcadValuePtr.h"
#include <algorithm>
#include <string>
#include <limits>

namespace Gcad {
namespace Utilities {

/**
  @brief
    Implementacja tablicy symboli indeksowana ciagami znakowymi,
    wykorzystujaca strukture reprezentujaca trynarne drzewo poszukiwan
    (Trynary Search Tree)

    Implementacja zostala oparta na przykladzie tablicy symboli
    przedstawionej w "Algorytmy C++ Grafy" R. Sedgewick'a, str. 47

  @remark
    Wyszczegolniona realizacja tablicy poszukiwan moze okazac sie 
    niewystarczajaca pod wzgledem wydajnosciowym. Uzycie w jej
    wewnetrznej reprezentacji sprytnych wskaznikow - opartych na zachowaniu
    semantyki tworzenia kopii elementow, za ktore sa odpowiedzialne -
    moze powodowac znaczne przeciazenia czasowe, implikowane wywolaniami
    konstruktorow kopiujacych, czy operatorow przypisania    
  
  @code
    #include "GcadSymbolTST.h"
    #include <iostream>

    using namespace Gcad::Utilities;
    using namespace std;

    typedef SymbolTST< char >  TST;

    void printHandle( TST*                trynaryTree,
                      const std::string&  symbol )
    {
      cout << "Symbol: " << symbol << ", Uchwyt: " <<
        trynaryTree->getHandle( symbol ) << endl;
    }

    int main()
    {
      // Konstruktor domyslny inicjalizuje wewnetrzna strukture w celu
      // przygotowania skladowych do przyjmowania komunikatow od klienta

      TST handleDispatcher2;

      {
        // Tworzymy druga instancje drzewa trynarnego, do ktorego bedziemy
        // wysylac zadania przypisania symbolom kolejnych uchwytow

        TST handleDispatcher1;

        cout << "*handleDispatcher1:\n";
        printHandle( &handleDispatcher1, "Ania" );
        printHandle( &handleDispatcher1, "Marek" );
        printHandle( &handleDispatcher1, "Malgosia" );
        printHandle( &handleDispatcher1, "Bartek" );
        printHandle( &handleDispatcher1, "Krzysztof" );
        printHandle( &handleDispatcher1, "Ania" );
        printHandle( &handleDispatcher1, "Bartek" );

        // Wykonujemy operacje przypisania dwoch odmiennych struktur 
        // drzew trynarnych, ktore zawieraja odmienne dane. Kazda 
        // z reprezentacji dziala na wlasnym zbiorze elementow, oraz jest
        // odpowiedzialna za przydzial oraz zwolnienie pamieci dla 
        // tworzonych wewnetrznie struktur

        handleDispatcher2 = handleDispatcher1;
      }
      // Konczy sie czas zycia obiektu automatycznego handleDispatcher1.
      // Jego wewnetrzna reprezentacja jest automatycznie czyszczona.
      // Zachowanie to nie daje zadnych skutkow ubocznych w postaci
      // blednych odwolan w obiekcie handleDispatcher2, poniewaz pracuje
      // on na wlasnych danych skladowych elementach

      cout << "\n*handleDispatcher2:\n";
      printHandle( &handleDispatcher2, "Ania" );
      printHandle( &handleDispatcher2, "Bartek" );
    }
  @endcode
*/
template< typename CHARACTER,
          typename HANDLE = size_t >
class SymbolTST {
 public:
   /** 
     @brief Typ determinujacy wartosci uchwytow zadanych symboli 
   */
   typedef HANDLE  Handle;

   /**
     @brief Deklaracja konkretyzowanego typu instancji symboli
   */
   typedef CHARACTER  SymbolChar;

   /** 
     @brief 
       Typ determinujacy rodzaj lancucha (char, lub wchar_t), ktorego
       instancje reprezentuja mapowane na uchwyty wartosci symboliczne
   */
   typedef std::basic_string< CHARACTER >   String;
   typedef typename String::const_iterator  StringConstItor;

 private:
   struct Node;

   /**
     @brief 
       Typ instancji o charakterystyce opakowujacej automatyczne 
       zachowanie kopiowania obiektow wskaznikow
   */
   typedef ValuePtr< Node >  NodeValuePtr;

   /**
     @brief 
       Podstawowy budulec drzewa - wezel z trzema laczeniami, oraz 
       skojarzona wartoscia opisujaca uchwyt danego symbolu
     
     @remark
       Oznaczenie podzbioru, nalezacego do struktury drzewa
       wyszczegolnionego ciagu znakow, jest wykonane poprzez 
       przypisanie wartosci maksymalnej danego typu uchwytu Handle.
       W rezultacie kolekcje znakow - reprezentujacych odwzorowywana
       na uchwyt symboliczna nazwe - w postaci sekwencyjnie uszeregowanych
       wezlow, zawieraja na ostatniej pozycji ciagu reprezentatywna
       wartosc uchwytu determinujaca nazwany symbol
   */
   struct Node {
     Node( SymbolChar symbol )
       : left_( 0 )
       , middle_( 0 )
       , right_( 0 )
       , handle_( std::numeric_limits< Handle >::max() )
       , symbol_( symbol )
     {}

     /**
       @brief 
         Sprawdzenie, w odpowiedzi na zapytanie, zawierania 
         na danej pozycji (czyli w danym wezle) uchwytu
     */
     bool isHandleAvaible() const { 
       return handle_ != std::numeric_limits< Handle >::max(); 
     }

     // Laczniki wezla struktury drzewa trynarnego
     NodeValuePtr  left_;
     NodeValuePtr  middle_;
     NodeValuePtr  right_;

     Handle        handle_; /**< Wartosc uchwytu danego wezla */
     SymbolChar    symbol_; /**< Wartosc symbolu zapisanego w danym wezle */
   };

 public:
   /**
     @brief Domyslny konstruktor inicjalizujacy wewnetrzna reprezentacje
   */
   SymbolTST()
     : head_( 0 )
     , handlePool_( 0 )
     , handleToReturn_( 0 )
   {}

   /**
     @brief 
       Wykonanie bezpiecznej - gwarantujacej poprawnosc w kazdych
       warunkach - operacji podmiany wewnetrznych danych
   */
   void swap( SymbolTST& rhs );

   /**
     @brief
       Na podstawie przekazanego argumentu wywolania, w postaci wartosci
       symbolu, metoda zwraca niepowtarzalny uchwyt z nim sprzezony
   */
   Handle getHandle( const String&  identifier );

   /**
     @brief
       Uzyskanie informacji dotyczacej istnienia (symetrycznie zaprzeczenia
       istnienia) wartosci zagregowanej w wewnetrznej strukturze, ktora moze 
       zostac odwzorowana na podstawie zadanego identyfikatora przekazanego
       w formie argumentu wywolania
   */
   bool isIdMapped( const String&  identifier );

 private:
   /**
     @brief
       Poprzez rekurencyjne wywolania metody, zostaje sprawdzone, ewentualnie
       dobudowane drzewo trynarne poszukiwan.
       
       Kolejne podzbiory ciagu, przekazanego za posrednictwem pary iteratorow,
       sa przekazywane do kolejnych wywolan funkcji, wewnatrz ktorej nastepuje
       analiza jego poszczegolnych znakow. W jednej iteracji jest sprawdzany
       dokladnie jedna wartosc, na podstawie ktorej budowana jest konstrukcja
       oparta na strukturze drzewa. Gdy trawersacja wszystkich elementow
       symbolu dobiegnie konca, wowczas nastepuja powroty z rekurencji, ktore
       ewentualnie wykonuja prace polaczenia utworzonych wezlow, do glownej
       struktury organizacyjnej.
   */
   NodeValuePtr& generateHandle( NodeValuePtr&    node,
                                 StringConstItor  beginSymbol,
                                 StringConstItor  endSymbol );

   /**
     @brief 
       Rekurencyjnie wywolywana pomocnicza funkcja uzywana 
       w implementacji metody isIdMapped
   */
   bool checkId( const String&  identifier,
                 size_t         charIndex,
                 NodeValuePtr&  node );

 private:
   NodeValuePtr  head_;           /**< Korzen drzewa trynarnego */
   Handle        handlePool_;     /**< Ostatnia zachowana wartosc uchwytu */
   Handle        handleToReturn_; /**< Wartosc uchwytu zadanego symbolu */
}; 


//
template< typename CHARACTER, typename HANDLE >
void 
SymbolTST< CHARACTER, HANDLE >
::swap( SymbolTST& rhs )
{
  std::swap( handlePool_, rhs.handlePool_ );
  std::swap( handleToReturn_, rhs.handleToReturn_ );
  head_.swap( rhs.head_ );
}

//
template< typename CHARACTER, typename HANDLE >
typename SymbolTST< CHARACTER, HANDLE >::Handle 
SymbolTST< CHARACTER, HANDLE >
::getHandle( const String&  identifier )
{
  head_ = NodeValuePtr( 
    generateHandle( head_, identifier.begin(), identifier.end() ) );
  return handleToReturn_;
}

//
template< typename CHARACTER, typename HANDLE >
typename SymbolTST< CHARACTER, HANDLE >::NodeValuePtr&
SymbolTST< CHARACTER, HANDLE >
::generateHandle( NodeValuePtr&    node,
                  StringConstItor  beginSymbol,
                  StringConstItor  endSymbol )
{
  StringConstItor  currSymbol = beginSymbol;
  SymbolChar       symbol     = *currSymbol;

  // Calkiem prawdopodobne, iz obiektowi sprytnego wskaznika brakuje
  // reprezentatywnego wezla (zmienna node) aktywnego symbolu (zmienna symbol).
  // Implikuje to sytuacje, gdy dana sciezka prowadzaca od korzenia drzewa
  // jest dziewicza. Efektem zaistnienia tej sytuacji jest potrzeba konstrukcji
  // nowych wezlow - tym samym budowa nowej sciezki - sprzezonych z aktualnie
  // przetwarzana encja symbolu

  if( node.empty() )
    node = NodeValuePtr( new Node( symbol ) );

  // Dotarcie do konca ciagu (poprzez rekurencyjne wywolania, zawezajace
  // zakres nazwy symbolicznej) implikuje wykonanie wyrazenia konczacego
  // rekursje. Sprawdzamy zawartosc ostatniego wezla, reprezentujacego
  // ostatni znak symbolu, determinujacego przypisany mu uchwyt. W sytuacji,
  // gdy dany ciag znakow jest nowym zbiorem uporzadkowanych elementow, 
  // musimy wygenerowac nowy uchwyt dla konczacego go wezla. Wartosc ta
  // nastepnie jest zapisywana w wewnetrznej strukturze klasy (handleToReturn),
  // aby umozliwic jej odczyt po powrocie z wywolan rekurencyjnych
  
  if( currSymbol == endSymbol ) {
    if( !node->isHandleAvaible() ) {
      node->handle_ = handlePool_++;
    }
    handleToReturn_ = node->handle_;
    return node;
  }

  // Powroty rekurencyjne - zeby byc scislym, wartosci przez nie zwracane -
  // sluza do powiazania wezlow drzewa. Nowo powstale wezly (znajdujace sie
  // na stosie wywolan), sa kolejno sprzezne, tworzac tym samym prawidlowa
  // strukture drzewa poszukiwan.
  // Kwestia do rozpatrzenia jest uzycie sprytnych wskaznikow. 
  // Najprawdopodobniej powoduja one spadek efektywnosciowy algorytmu,
  // podczas przypisan zwracanych odwolan do wezlow. Powodem zanizenia
  // czasu wykonania sa naprzemienne wywolania dekstruktow, konstruktorow
  // operatora przypisania. Jednak dopoki nie zostana wykonane testy empiryczne,
  // za posrednictwem programu profilujacego, stwierdzenie, iz stosunek korzysci
  // plynacej z uzycia sprytnych wskaznikow w celu automatyzacji kopiowania
  // (oraz bezpiecznej obslugi w wypadku powstania sytuacji wyjatkowych), 
  // do wartosci zmniejszenia wydajnosci implementacji, jest niedopuszczalny,
  // dopoty kwestia ta nie bedzie wplywac na realizacje algorytmu, oraz jego
  // struktury

  if( symbol < node->symbol_ ) {
    node->left_ = NodeValuePtr(
      generateHandle( node->left_, currSymbol, endSymbol ) );
  }

  if( symbol == node->symbol_ ) {
    node->middle_ = NodeValuePtr( 
      generateHandle( node->middle_, ++currSymbol, endSymbol ) );
  }

  if( symbol > node->symbol_ ) {
    node->right_ = NodeValuePtr(
      generateHandle( node->right_, currSymbol, endSymbol ) );
  }

  return node;
}

//
template< typename CHARACTER, typename HANDLE >
bool
SymbolTST< CHARACTER, HANDLE >
::isIdMapped( const String& identifier )
{
  const size_t FIRST_ID_CHARACTER = 0;
  return checkId( identifier, FIRST_ID_CHARACTER, head_ );
}

//
template< typename CHARACTER, typename HANDLE >
bool
SymbolTST< CHARACTER, HANDLE >
::checkId( const String&  identifier,
           size_t         charIndex,
           NodeValuePtr&  node )
{
  if( node.empty() ) 
    return false;

  if( charIndex == identifier.size() &&
    node->isHandleAvaible() )
  {
    return true;
  }

  if( identifier[charIndex] < node->symbol_ ) {
    return checkId( identifier, charIndex, node->left_ );
  }
  else if( identifier[charIndex] == node->symbol_ ) {
    return checkId( identifier, charIndex + 1, node->middle_ );
  }
  else {
    return checkId( identifier, charIndex, node->right_ );
  }
}

} // namespace Utilities
} // namespace Gcad

#endif
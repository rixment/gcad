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

#ifndef _GCAD_ASSOCIATIVEMANAGER_H_
#define _GCAD_ASSOCIATIVEMANAGER_H_

#include "GcadAssertion.h"
#include "GcadException.h"
#include "GcadGenericKeyMapConstItor.h"
#include "GcadRefCountPtr.h"
#include <map>
#include <string>

namespace Gcad {
namespace Utilities {

/** 
  @brief 
    Szablon klas, przeznaczony do tworzenia rodziny menadzerow zasobow

  @param 
    RESOURCE Parametr szablonu okreslajacy typ elementow bedacych 
    zasobami zarzadzanymi przez instancje menadzera

  @param 
    KEY Parametr okreslajacy typ klucza sprzezonego z okreslonym zasobem

  @code
    #include "GcadAssociativeManager.h"
    #include <iostream>

    using namespace Gcad::Utilities;
    using namespace std;

    typedef AssociativeManager< int >   IntMgr;
    typedef IntMgr::ResourceKeyUnvalid  IntMgrResUnvalid;

    int main() try
    {
      {
        IntMgr  intMgr;
              
        intMgr.addResource( "jeden", new int( 1 ) );
        intMgr.addResource( "osiem", new int( 8 ) );
        intMgr.addResource( "piec", new int( 5 ) );

        cout << "osiem: " << intMgr.dereference( "osiem" ) << endl;
        cout << "jeden: " << intMgr.dereference( "jeden" ) << endl;

        intMgr.removeResource( "piec" );

        // Proba stworzenia kopii menadzera nie jest dozwolona
        
        // IntMgr  badMgr( intMgr ); 
        // badMgr = intMgr;
      }
      
      // Konczy sie czas zycia obiektu intMgr, ktory jest odpowiedzialny
      // za zwolnienie pamieci zasobow, ktorymi zarzadzal
    }
    catch( IntMgrResUnvalid& resUnvalid )
    {
      cerr << resUnvalid.what() << endl;
    }
  @endcode
*/
template< typename RESOURCE, 
          typename KEY = std::string > 
class AssociativeManager {
 public:
   /** 
     @brief Synonim typu inteligentnego wskaznika do zasobu
   */
   typedef RefCountPtr< RESOURCE >  ResourceCntPtr;

   /**
     @brief Synonim typu instancji kluczy
   */
   typedef KEY  KeyType;

   /** 
     @brief 
       Synonim kolekcji zasobow, zlozonego z elementow okreslajacych
       wartosci:
       - klucz zasobu
       - referencja do zasobu
   */
   typedef std::map< KeyType, ResourceCntPtr >  Resources;

   /** 
     @brief 
       Szablon klasy wyjatkow, zglaszanych podczas proby pozyskania 
       zasobu wywolanego z nieprawidlowym kluczem, oraz gdy nastapila
       prosba dodania zasobu, ktorego klucz znajduje sie juz w menadzerze
   */
   class ResourceKeyUnvalid : public Utilities::Exception {
    public:   
      friend class AssociativeManager;
      
      /** 
        @brief Akcesor blednego klucza 
      */
      const KeyType&  unvalidKey() const { return unvalidKey_; }

      /** 
        @brief 
          Akcesor do instancji menadzera, ktorego uzycie zainicjowalo
          powstanie sytuacji wyjatkowej
      */
      AssociativeManager&  manager() { return manager_; }
    
    private:
      ResourceKeyUnvalid( AssociativeManager*  manager,
                          const KeyType&       unvalidKey,
                          const char*          exceptionDescription )
        : Utilities::Exception( exceptionDescription )
        , manager_( *manager )
        , unvalidKey_( unvalidKey )
      {}
      
    private:
      /** 
        @brief Niepoprawny identyfikator 
      */
      KeyType  unvalidKey_;

      /** 
        @brief Menadzer, ktory spowodowal wyrzucenie sytuacji wyjatkowej 
      */
      AssociativeManager&  manager_;
   };

   /**
     @brief
       Iterator dostepu do zarejestrowanych identifikatorow
   */
   class IDsConstItor
     : public GenericKeyMapConstItor< Resources > 
   {
      friend class AssociativeManager;

      IDsConstItor( typename Resources::const_iterator  itor )
        : GenericKeyMapConstItor< Resources >( itor )
      {}
   };

   /** 
     @brief Domyslny konstruktor 
     
     @remark 
       Istnienie domyslnego konstruktora jest implikowane przez
       zadeklarowanie w sekcji prywatnej konstruktora kopiujacego
   */
   AssociativeManager() {}
  
   /** 
     @brief 
       Dodanie do menadzera nowego zasobu, wraz z odpowiadajacym mu kluczem

     @param 
       idKey Klucz identyfikujacy zasob

     @param 
       resource Adres zasobu utworzonego w stercie

     @remark 
       W przypadku niemoznosci umieszczenia zasobu w menadzeze, 
       spowodowanej wystapieniem kolizji pomiedzy identyfikatorami (dodawanym
       do menadzera, oraz w nim wystepujacym), zostaje rzucony wyjatek
       typu ResourceKeyUnvalid

     @exception
       ResourceKeyUnvalid
   */
   void addResource( const KeyType&  idKey, 
                     RESOURCE*       resource );

   /** 
     @brief 
       Usuwa zasob z menadzera, wraz z towarzyszacym mu identyfikatorem
     
     @return 
       Gdy zasob identyfikowany poprzez podany klucz istnieje,
       wowczas zostaje usuniety, a metoda zwraca wartosc prawda. 
       W przeciwnym wypadku nie jest podejmowane zadne dzialanie, a metoda
       zwraca falsz
   */
   bool removeResource( const KeyType& idKey );

   /** 
     @brief 
       Pozyskanie zasobu znajdujacego sie w biezacym menadzeze,
       identyfikowanego na podstawie przeslanego klucza

     @remark 
       Funkcja zglasza sytuacje wyjatkowa typu 
       AssociativeManager::ResourceKeyUnvalid, gdy wartosc przekazanego 
       klucza nie moze byc odwzorowana na zasob zawarty w menadzerze

     @exception
       ResourceKeyUnvalid
   */
   RESOURCE& dereference( const KeyType& idKey );

   /** 
     @brief 
       Za posrednictwem metody mozna dowiedziec sie, czy
       zasob o podanym identyfikatorze posiada reprezentacje
       w menadzerze odbierajacym komunikat
   */
   bool isResourceAvaible( const KEY& idKey ) const;

   /** 
     @brief 
       Usuniecie wszystkich zasobow, przechowywanych w menadzeze
   */
   void removeAll();
   
   /** 
     @brief 
       Ilosc zasobow aktualnie znajdujacych sie pod kontrola menadzera
   */
   size_t size() const;

   /**
     @brief 
       Udostepnienie iteratora do kolekcji zarejestrowanych identyfikatorow

       Poprzez wywolanie metod begin(), oraz end(), otrzymujemy za posrednictwem
       iteratorow pelen zakres elementow zawartych w menadzerze. Iterujac
       kolejno przez zawarte w nim identyfikatory, mozemy za pomoca metody
       dereference() uzyskac dostep do wszystkich zagregowanych elementow 
   */
   IDsConstItor  begin() const { return resources_.begin(); }
   IDsConstItor  end()   const { return resources_.end(); }

 private:
   Resources  resources_;

 private:
   // Nie zaimplementowane
   AssociativeManager( const AssociativeManager& );
   AssociativeManager& operator =( const AssociativeManager& );
};


//
template< typename RESOURCE, typename KEY > 
void 
AssociativeManager< RESOURCE, KEY >
::addResource( const KeyType&  idKey,
               RESOURCE*       resource )
{
  assertion( resource != 0, 
    "Przypisany adres zasobu ma niepoprawna wartosc" );
  
  // Sprawdz, czy menadzer posiada juz podany klucz idKey. Jesli okaze sie, iz
  // takiego klucza nie znalazl, wowczas dodaj zasob. W przeciwnym wypadku 
  // oznajmij o zaistnieniu sytuacji wyjatkowej, spowodowanej proba dodania
  // zasobu o identyfikatorze znajdujacym sie juz w menadzerze
  if( resources_.count( idKey ) == 0 ) 
    resources_.insert( make_pair( idKey, ResourceCntPtr( resource ) ) );
  else {
    throw ResourceKeyUnvalid( 
      this, 
      idKey, 
      "Proba dodania zasobu o identyfikatorze znajdujacym sie w menadzerze" );
  }
}

//
template< typename RESOURCE, typename KEY > 
bool 
AssociativeManager< RESOURCE, KEY >
::removeResource( const KeyType& idKey )
{
  // Sprawdz, czy menadzer jest w posiadaniu zasobu o podanym kluczu idKey.
  // W przypadku odnalezienia zasobu okreslonego danym kluczem, nalezy go
  // usunac z menadzera i powiadomic o poprawnym przeprowadzeniu dzialania.
  // Gdy zasob nie znajduje sie w menadzeze, przekaz informacje o niepowodzeniu
  // dzialania usuniecia zasobu o podanym kluczu idKey
  return resources_.erase( idKey ) != 0;
}

//
template< typename RESOURCE, typename KEY > 
RESOURCE& 
AssociativeManager< RESOURCE, KEY >
::dereference( const KeyType& idKey ) 
{
  // Jezeli w menadzerze znajduje sie zasob identyfikowany przez idKey, wowczas
  // zwroc go do funkcji wywolujacej. W przeciwnym wypadku poinformuj 
  // o niepowodzeniu, poprzez wyrzucenie sytuacji wyjatkowej
  typename Resources::iterator founded = resources_.find( idKey );

  if( founded == resources_.end() ) {
    throw ResourceKeyUnvalid( 
      this, 
      idKey, 
      "Dereferencja zasobu o nieprawidlowym kluczu" );
  }
  return *( founded->second );
}

//
template< typename RESOURCE, typename KEY > 
bool 
AssociativeManager< RESOURCE, KEY >
::isResourceAvaible( const KEY& idKey ) const
{
  return resources_.count( idKey ) != 0;
}

//
template< typename RESOURCE, typename KEY > 
void 
AssociativeManager< RESOURCE, KEY >
::removeAll()
{
  Resources().swap( resources_ );
}

//
template< typename RESOURCE, typename KEY > 
size_t 
AssociativeManager< RESOURCE, KEY >
::size() const 
{
  return resources_.size();
}

} // namespace Utilities
} // namespace Gcad

#endif
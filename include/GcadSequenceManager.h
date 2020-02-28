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

#ifndef _GCAD_SEQUENCEMANAGER_H_
#define _GCAD_SEQUENCEMANAGER_H_

#include "GcadRefCountPtr.h"
#include "GcadException.h"
#include <string>
#include <vector>
#include <utility>

namespace Gcad {
namespace Utilities {

/** 
  @brief
    Implementacja menadzera zachowujacego kolejnosc przechowywanych elementow

    Agregat typu SequenceManager sluzy do przechowywania, oraz wykorzystuje
    mechanizmy zarzadzania elementami bedacymi typami RESOURCE. Mozliwe jest 
    przechowywanie podtypow, poniewaz wewnetrznie agregat przechowuje zasoby 
    w postaci wskaznikow. Kolejnosc wystepowania poszczegolnych jednostek 
    w kolekcji jest ustalana przez klienta, ktory ma do dyspozycji metody 
    pozwalajace segregowac umieszczane elementy. Zarzadca posiada pelne 
    prawa wlasnosci do agregatow, ktore sa nadawane w czasie ich przypisywania 
    do jego struktury. Destrukcja obiektu menadzera jest rownoznaczna 
    dezalokacji wszystkich elementow w nim zawartych, poprzez wywolanie 
    odpowiednich destruktorow agregatow.

  @remark
    Nalezy pamietac, iz wykorzystujac kontener typu vector, lub deque do 
    reprezentowania wewnetrznej struktury agregatu, podczas dodawania, lub
    usuwania elementow z zarzadcy wszystkie zewnetrznie przechowywane
    iteratory moga zostac uniewaznione. Sytuacja ta implikowana jest semantyka
    tego rodzaju kontenerow, ktore dynamicznie powiekszaja obszar pamieci
    potrzebnej do przechowania elementow. 
    Tych dolegliwosci nie posiada struktura listy, ktora w czasie dodawania
    lub usuwania wezlow reprezentujacych elementy nie przemieszcza pozostalych
    pozycji w celu dostosowania reprezentacji do poprawnego stanu

  @param 
    RESOURCE Determinuje typ agregowanych obiektow

  @param
    KEY_ID Determinuje typ identyfikujacy agregowany obiekt

  @param 
    CONTAINER Determinuje kontener wykorzystywany w wewnetrznej implementacji.
    Jego typ powinien umozliwiac umieszczanie elementow na koncu kolekcji, 
    oraz w ustalonym przez iterator miejscu - typowe operacje dla kontenerow
    sekwencyjnych, takich jak: vector, deque, list

  @code
#include "GcadSequenceManager.h"
#include <iostream>

using namespace Gcad::Utilities;
using namespace std;

class Base { 
 public: 
   Base( const string& id )
     : id_( id ) 
   {}
   virtual ~Base() {}
   virtual string whatType() const { return "Base"; } 
   string id() const { return id_; }

 private: 
   string id_;
};

class Derived : public Base {
 public:
   Derived( const string& id )
     : Base( id )
   {}
   virtual string whatType() const { return "Derived"; } 
};

typedef SequenceManager< Base > Manager;

void printManager( const Manager& manager )
{
  for( Manager::ResourceConstItor curr = manager.begin();
    curr != manager.end();
    ++curr )
  {
    cout << "TYP: " << curr->whatType() << 
      " INFO: " << curr->id() << endl;
  }
  cout << endl;
}


int main()
{
  Manager manager;

  manager.addBack( new Base( "Bazowy1" ), "B1Identifier" );
  manager.addBack( new Derived( "Dziedzic1" ), "D1" );
  manager.addBack( new Derived( "Dziedzic2" ), "D2" );
  manager.addBack( new Derived( "Dziedzic3" ), "D3" );

  printManager( manager );

  manager.add( new Base( "Baza2" ), "B2", ++manager.begin() );
  manager.add( new Derived( "Dziedzic4" ), "D4", ++manager.begin() );

  printManager( manager );

  manager.remove( manager.begin() );
  manager.remove( "D3" );

  printManager( manager );

  try {
    cout << manager.dereference( "B1Identifier" ).id() << endl;
  } 
  catch( Manager::BadIdentifier& )
  {
    // Klauzula obslugi wyjatku powstalego w czasie proby dopasowania
    // przekazanego jako parametr metody dereference blednego identyfikatora
  }

  cout << manager.dereference( "D4" ).id() << endl;
  cout << "Rozmiar menadzera: " << manager.size() << endl;

  system( "pause" );
}
  @endcode
*/
template< 
  typename RESOURCE,
  typename KEY_ID    = std::string,
  typename CONTAINER = std::vector<
    std::pair< KEY_ID, RefCountPtr< RESOURCE > > 
  >
>
class SequenceManager {
 public:
   typedef RESOURCE                              Resource;
   typedef Resource*                             ResourcePtr;
   typedef KEY_ID                                ResourceId;
   typedef std::pair< ResourceId, ResourcePtr >  ResourcePair;
   typedef RefCountPtr< Resource >               ResourceRefPtr;
   typedef CONTAINER                             Container;

   /**
     @brief
       Zwykly iterator udostepniajacy agregowane elementy
   */
   class ResourceItor {
    private:
      friend class SequenceManager;
      friend class ResourceConstItor;
    
      explicit ResourceItor( typename Container::iterator itor )
        : itor_( itor )
      {}
    
    public:
      Resource& operator *() const {
        return *itor_->second;
      }

      Resource* operator ->() const {
        return itor_->second.get();
      }

      ResourceItor& operator ++() {
        ++itor_;
        return *this;
      }

      ResourceItor  operator ++( int ) {
        ResourceItor temp( itor_ );
        ++( *this );
        return temp;
      }

      bool operator !=( ResourceItor rhs ) const { 
        return itor_ != rhs.itor_;
      }

    private:
      typename Container::iterator  itor_;
   };
   
   /**
     @brief
       Iterator do agregatow opatrzonych specyfikatorem const
   */
   class ResourceConstItor {
    private:
      friend class SequenceManager;
       
      explicit ResourceConstItor( typename Container::const_iterator itor )
        : itor_( itor )
      {}
    
    public:
      explicit ResourceConstItor( ResourceItor itor )
        : itor_( itor.itor_ )
      {}

      const Resource& operator *() const {
        return *itor_->second;
      }

      const Resource* operator ->() const {
        return itor_->second.get();
      }

      ResourceConstItor& operator ++() {
        ++itor_;
        return *this;
      }

      ResourceConstItor  operator ++( int ) {
        ResourceItor temp( itor_ );
        ++( *this );
        return temp;
      }

      bool operator !=( ResourceConstItor rhs ) const { 
        return itor_ != rhs.itor_;
      }

    private:
      typename Container::const_iterator  itor_;
   };

   /** 
     @brief 
       Obiekt wyjatku powstalego w sytuacji bledu dopasowania 
       przekazanego przez klienta identyfikatora
   */
   class BadIdentifier : public Utilities::Exception {
    public:
      BadIdentifier( const ResourceId&       badIdentifier,
                     const SequenceManager&  whichManager )
        : Utilities::Exception("Nie mozna dopasowac identyfikatora!")
        , badIdentifier_( badIdentifier )
        , whichManager_( whichManager )
      {}
      
      ResourceId&             badIdentifier() { return badIdentifier_; }
      const SequenceManager&  whichManager()  { return whichManager_; }

    private:
      ResourceId              badIdentifier_;
      const SequenceManager&  whichManager_;      
   };

   /** 
     @brief 
       Zablokowanie konstruktora kopiujacego implikuje pusta definicje domyslnego
   */
   SequenceManager() {}

   /** 
     @brief 
       Uzyskanie na podstawie wartosci identyfikatora pozadanego zasobu
   */
   Resource& dereference( const ResourceId& resourceId );
   
   /** 
     @brief 
       Uzyskanie na podstawie wartosci identyfikatora pozadanego zasobu
   */
   const Resource& dereference( const ResourceId& resourceId ) const;

   /** 
     @brief 
       Dodanie nowego zasobu na koncowej pozycji menadzera

     @remark
       Mozliwe jest dodanie zasobu wraz ze skojarzonym z nim identyfikatorem,
       przyczym wartosc go identyfikujaca moze byc zdublowana
   */
   void addBack( ResourcePtr        resource,
                 const ResourceId&  identifier );
   
   /** 
     @brief
       Dodanie zasobu na pozycji okreslonej przez iterator       
   */
   void add( ResourcePtr        resource,
             const ResourceId&  identifier,
             ResourceItor       where );

   /** 
     @brief
       Usuniecie z menadzera - pierwszego dopasowanego do wartosci 
       identyfikatora - zasobu
   */
   void remove( const ResourceId&  resourceId );

   /** 
     @brief
       Usuniecie zasobu wskazywanego przez iterator
   */
   void remove( ResourceItor  itor );
   
   /** 
     @brief
       Oproznienie menadzera ze wszystkich agregowanych zasobow
   */
   void removeAll();

   /** 
     @brief
       Umozliwienie iteracyjnego przechodzenia kolekcji
   */
   ResourceItor begin();
   ResourceItor end();

   ResourceConstItor begin() const;
   ResourceConstItor end()   const;

   /**
     @brief
       Udostepnienie informacji o wielkosci agregatu - ilosci agregowanych
       elementow
   */
   size_t size() const;

 private:
   Container  resources_;

   Resource& resourceDereference( const ResourceId& resourceId ) const;

 private:
   // Nie zaimplementowane
   SequenceManager( const SequenceManager& );
   SequenceManager& operator =( const SequenceManager& );
};


//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::Resource& 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::resourceDereference( const ResourceId& resourceId ) const
{
  for( typename Container::const_iterator curr = resources_.begin();
    curr != resources_.end();
    ++curr )
  {
    if( curr->first == resourceId )
      return *curr->second;
  }
  
  throw BadIdentifier( resourceId, *this );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::Resource& 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::dereference( const ResourceId& resourceId )
{
  return resourceDereference( resourceId );
}
   
//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
const typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::Resource& 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::dereference( const ResourceId& resourceId ) const
{
  return resourceDereference( resourceId );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
void 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::addBack( ResourcePtr        resource,
           const ResourceId&  identifier )
{
  resources_.push_back( ResourcePair( identifier, resource ) );
}
   
//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
void 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::add( ResourcePtr        resource,
       const ResourceId&  identifier,
       ResourceItor       where )
{
  resources_.insert( where.itor_, ResourcePair( identifier, resource ) );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
void 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::remove( const ResourceId&  resourceId )
{
  for( typename Container::iterator curr = resources_.begin();
    curr != resources_.end();
    ++curr )
  {
    if( curr->first == resourceId ) {
      resources_.erase( curr );
      return;
    }      
  }
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
void 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::remove( ResourceItor  itor )
{
  resources_.erase( itor.itor_ );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
void 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::removeAll()
{
  Container().swap( resources_ );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::ResourceItor 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::begin()
{
  return ResourceItor( resources_.begin() );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::ResourceItor 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::end()
{
  return ResourceItor( resources_.end() );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::ResourceConstItor 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::begin() const
{
  return ResourceConstItor( resources_.begin() );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
typename SequenceManager< RESOURCE, KEY_ID, CONTAINER >::ResourceConstItor 
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::end() const
{
  return ResourceConstItor( resources_.end() );
}

//
template< typename RESOURCE, typename KEY_ID, typename CONTAINER >
size_t
SequenceManager< RESOURCE, KEY_ID, CONTAINER >
::size() const
{
  return resources_.size();
}

} // namespace Utilities
} // namespace Gcad

#endif
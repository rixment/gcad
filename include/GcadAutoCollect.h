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

#ifndef _GCAD_AUTOCOLLECT_H_
#define _GCAD_AUTOCOLLECT_H_

#include <set>

namespace Gcad {
namespace Utilities {

/** 
  @brief
    Szablon reprezentuje odmiane wzorca automatycznych list

    Szablon reprezentuje odmiane wzorca automatycznych list.
    W czasie tworzenia obiektow klas dziedziczacych z klas powstalych 
    z szablonu AutoCollect, ich adresy sa automatycznie zachowywane 
    w kolekcji odwzorowanej dla danej konkretyzacji szablonu.
    Niezaleznie od rodzaju pamieci, w ktorej dany obiekt zostaje utworzony,
    jego adres zostaje zapamietany we wspomnianym kontenerze. Usuniecie adresu
    rowniez zostaje wykonane automatycznie - podczas destrukcji obiektu.<br>
    Umieszczenie konstruktorow w sekcji chronionej daje gwarancje, iz zadna 
    konkretna instancja klasy AutoCollect nie zostanie powolana do zycia.
  
  @remark 
    Nalezy zworcic uwage, iz destruktor nie jest wirtualny. Klasa
    AutoCollect udostepnia jedynie implementacje, dzieki ktorej utworzone 
    obiekty sa automatycznie rejestrowane w kontenerze. Klient ma dostep 
    do elementow kolekcji za posrednictwem iteratorow. Warto nadmienic, iz
    obiekty przechowywane w kolekcji nie sa posegregowane w sposob 
    kolejnosci ich tworzenia. Kolejnosc ta jest przypadkowa i zalezy od
    rodzaju pamieci, w ktorej alokowany jest obiekt<br>
    Wykorzystanie klasy, jako interfejsu jest niepoprawne. Szablon udostepnia 
    implementacje, nie interfejs!<br>
  
  @param 
    T Pochodna klasa wykorzystujaca funkcjonalnosc AutoCollect

  @code
    #include "GcadAutoCollect.h"
    #include <iostream>
    #include <string>
    #include <memory>

    using namespace Gcad::Utilities;
    using namespace std;

    class InstancesAutoCollect : public AutoCollect< InstancesAutoCollect > {
     public:
       InstancesAutoCollect( const string& print )
         : print_( print ) {}
       
       string print() { reurn cout << print_.c_str() << endl; }

     private:       
       string print_;
    };

    InstancesAutoCollect global( "GLOBAL" );

    void main()
    {
      InstancesAutoCollect local( "LOCAL" );

      auto_ptr< InstancesAutoCollect > heap( new InstancesAutoCollect("HEAP") );
      
      typedef InstancesAutoCollect::AdressCollectionItor AutoCollectItor;

      for( AutoCollectItor curr = InstancesAutoCollect::begin();
        curr != InstancesAutoCollect::end();
        ++curr )
      {
        ( *curr )->print();
      }
    }
  @endcode
*/
template< typename T > class AutoCollect {
 public:
   /**
     @brief
       Kolekcja adresow aktualnie istniejacych obiektow,
       bedacych podtypami AutoCollect< T >
   */
   typedef std::set< T* >                        AdressCollection;
   typedef typename AdressCollection::iterator   AdressCollectionItor;

   /** 
     @brief 
       Okresla poczatek zakresu kolekcji

     @return 
       Iterator do poczatku kolekcji wskaznikow obiektow
   */
   static AdressCollectionItor begin();
   
   /** 
     @brief 
       Okresla koniec zakresu kolekcji
     
     @return 
       Iterator umiejscowiony "za koncem" kolekcji
   */
   static AdressCollectionItor end();

 protected:
   // Udostepnienie konstruktorow i destruktora jedynie klasom pochodnym
   AutoCollect();
   AutoCollect( const AutoCollect& );
   ~AutoCollect();

 private:
   /** 
     @brief 
       Wykorzystanie wlasciwosci wzorca singleton, w celu poprawnej 
       inicjalizacji kontenera, przed jego pierwszym uzyciem 
   */
   static AdressCollection& dataSet();

   /** 
     @brief 
       Umieszczenie wewnatrz kolekcji adresu obiektu, 
       dla ktorego zostala wywolana metoda 
   */
   void insert();
};


//
template< typename T > 
AutoCollect< T >::AdressCollection& AutoCollect< T >
::dataSet()
{
  // Zbior przechowujacy adresy obiektow klas pochodnych od 
  // skonkretyzowanej klasy szablonowej AutoCollect
  static AdressCollection adressCollection;
  return adressCollection;
}

//
template< typename T > inline void AutoCollect< T >
::insert()
{
  // Typem statycznym wskaznika this jest AutoCollect< T >, 
  // wiec nalezy wykonac rzutowanie w dol hierarchii, poniewaz
  // zbior elementow przechowuje wskazniki klas pochodnych.
  // Typ T powinien publicznie dziedziczyc po klasie AutoCollect< T >,
  // w przeciwnym przypadku proba jawnej konwersji typu bedzie powodowac
  // wystapienie bledu na etapie kompilacji
  T* const This = static_cast< T* >( this );
  dataSet().insert( This );
}

//
template< typename T > AutoCollect< T >
::AutoCollect() 
{
  insert();
}

//
template< typename T > AutoCollect< T >
::AutoCollect( const AutoCollect& ) 
{
  insert();
}

//
template< typename T > AutoCollect< T >
::~AutoCollect() 
{
  T* const This = static_cast< T* >( this );
  dataSet().erase( dataSet().find( This ) );
}

//
template< typename T > 
typename AutoCollect< T >::AdressCollectionItor AutoCollect< T >
::begin()
{
  return dataSet().begin();
}

//
template< typename T > 
typename AutoCollect< T >::AdressCollectionItor AutoCollect< T >
::end()
{
  return dataSet().end();
}

} // namespace Utilities
} // namespace Gcad

#endif

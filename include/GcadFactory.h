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

#ifndef _GCAD_FACTORY_H_
#define _GCAD_FACTORY_H_

#include "GcadException.h"
#include <map>

namespace Gcad {
namespace Utilities {

/** 
  @brief 
    Szablon prezentujacy uogolniony wzorzec projektowy - fabryka obiektow

  @remark 
    Aby mogla byc spelniona funkcjonalnosc fabryki, typy tworzonych
    instancji musza udostepniac konstruktor domniemany
  
  @param 
    T Bazowy typ tworzonych obiektow
  
  @param 
    ID_KEY Typ klasy determinujacej klucz do funkcji fabrycznej obiektu

  @code
    #include "GcadFactory.h"
    #include <iostream>
    #include <string>
    #include <memory>

    using namespace Gcad::Utilities;
    using namespace std;

    class B { 
     public:
       virtual ~B() {}
    };

    class D1 : public B {};
    class D2 : public B {};

    void main()
    {
      Factory< B, string >  factory;
      
      factory.reg< D1 >( "D1" );
      factory.reg< D2 >( "D2" );

      auto_ptr< B > base[] = { 
        auto_ptr< B >( factory.create( "D1" ) ),
        auto_ptr< B >( factory.create( "D2" ) )
      };

      factory.unreg( "D1" );

      cout << "Registered IDs: ";
      for( Factory< B, string >::IDsConstItor idItor = factory.begin();
        idItor != factory.end();
        ++idItor )
      {
        cout << ( *idItor ).c_str() << " ";
      }
    }
  @endcode
*/
template< typename T, 
          typename ID_KEY > 
class Factory {
 private:
   /** 
     @brief Synonim wskaznika do funkcji tworzacych instancje obiektow
   */
   typedef T* ( *ConstrFnPtr )();

   /** 
     @brief 
       Synonim kontenera odwzorowujacego identyfikatory, na funkcje fabryczne
   */
   typedef std::map< ID_KEY, ConstrFnPtr >  Data;

 public:
   /** 
     @brief 
       Synonim iteratora umozliwiajacego iteracyjna trawersacje
       przez elementy kolekcji 
   */
   class IDsConstItor {
    public:
      /**
        @brief Konstruktor inicjalizujacy wewnetrzny iterator
      */
      explicit IDsConstItor( typename Data::const_iterator itor )
        : itor_( itor )
      {}

      /**
        @brief Pre-inkrementacja iteratora
      */
      IDsConstItor& operator ++() {
        ++itor_;
        return *this;
      }

      /**
        @brief Post-inkrementacja iteratora
      */
      IDsConstItor operator ++( int ) {
        IDsConstItor beforeItor = itor_;
        this->operator ++();
        return beforeItor;
      }

      /**
        @brief Wyluskanie identyfikatora zasobu
      */
      ID_KEY operator *() const {
        return itor_->first;
      }

      /**
        @brief Funkcja tozsamosci dwoch iteratorow
      */
      bool operator ==( const IDsConstItor&  rhs ) const {
				return itor_ == rhs.itor_;
			}

      /**
        @brief Funkcja relacji roznicy dwoch iteratorow
      */
      bool operator !=( const IDsConstItor&  rhs ) const {
				return !( *this == rhs );
			}

    private:
      typename Data::const_iterator  itor_;
   };

   /** 
     @brief 
       Klasa wyjatku, ktory zostaje rzucony w sytuacji niemoznosci 
       odwzorowania wartosci przekazanego identyfikatora do metody 
       Factory::create, na funkcje tworzaca dana instancje obiektu
   */
   class UnvalidID : public Utilities::Exception {
    public:
      UnvalidID( const ID_KEY& notFoundedID )
        : Utilities::Exception("Fabryka otrzymala nieprawidlowy identyfikator!")
        , notFoundedID_( notFoundedID ) {}

      /** 
        @brief 
          Metode nalezy wywolac, kiedy chcemy dowiedziec sie 
          jaki identyfikator spowodowal wystapienie sytuacji wyjatkowej
      */
      ID_KEY getID() const { return notFoundedID_; }
      
    private:
      /** 
        @brief Bledny identyfikator 
      */
      ID_KEY  notFoundedID_;
   };

   /** 
     @brief 
       Klasa wyjatku, ktory moze wystapic podczas rejestracji funkcji 
       fabrycznej, gdy przekazany identyfikator, sluzacy do odwzorowania 
       rejestrowanej funkcji, juz istnieje w "swiadomosci" fabryki
   */
   class DoubledID : public Utilities::Exception {
    public:
      DoubledID( const ID_KEY& doubledID )
        : Utilities::Exception("Nastapila kolizja identyfikatorow!")
        , doubledID_( doubledID ) {}

      /** 
        @brief Informacja na temat zdublowanego identyfikatora
      */
      ID_KEY getID() const { return doubledID_; }
        
    private:
      /** 
        @brief Zdublowany identyfikator 
      */
      ID_KEY  doubledID_;
   };

   /** 
     @brief Domyslny konstruktor 
   */
   Factory() {}

   /** 
     @brief 
       Na podstawie przekazanego identyfikatora, metoda tworzy instancje 
       obiektu zaalokowanego w stercie, oraz zwraca jego poprawny adres

     @remark 
       Gdy przekazany parametr nie moze zostac odwzorowany na funkcje
       sluzaca do utworzenia danej instancji o wyszczegolnionym identyfikatorze,
       wowczas zostaje wyrzucony wyjatek typu Factory::UnvalidID.
   */
   T* create( const ID_KEY& id ) const;
   
   /** 
     @brief 
       Funkcja wiazaca typ U, wraz z identyfikatorem. Na podstawie 
       wywolania metody fabryka bedzie mogla mapowac dany identyfikator, 
       na odpowiadajaca mu funkcje tworzaca obiekt szczegolnego typu.
     
     @remark 
       W sytuacji, gdy przekazany identyfikator odwzorowywuje juz 
       hipotetyczna funkcje, wowczas metoda wyrzuca wyjatek typu 
       Factory::DoubledID, tym samym oznajmiajac, iz ponowna rejestracja dla 
       danego identyfikatora nie jest mozliwa
   */
   template< typename U > void 
     reg( const ID_KEY& id );
   
   /** 
     @brief 
       Metoda ma na celu odrejestrowanie funkcji tworzacej instancje obiektu
     
     @return 
       Zwraca prawde, gdy dezaktywacja funkcji zostala zakonczona
       pomyslnie. Natomiast jesli dana funkcja, oraz przypisany do niej 
       identyfikator nie byly znane fabryce, wowczas zwraca wartosc falsz
   */
   bool unreg( const ID_KEY& id );

   /** 
     @brief 
       Dzieki umozliwieniu korzystania z iteratorow, 
       klient ma sposobnosc wykonania sprawdzenia wartosci 
       identyfikatorow zarejestrowanych aktualnie w fabryce.
     
     @return Iterator do poczatku kolekcji wartosci:
       - nazwa identyfikujaca funkcje fabryczna
       - adres funkcji fabrycznej
   */
   IDsConstItor begin();

   /** 
     @see begin()
   */
   IDsConstItor end();
 
 private:
   /** 
     @brief 
       Kolekcja odwzorowujaca identyfikatory na funkcje fabryczne 
   */
   Data  fnPtrsCollection_;

   /** 
     @brief 
       Szablon sluzacy do tworzenia calej rodziny funkcji, ktorych 
       zadaniem jest kreowanie w stercie obiektow typow U 
       (wydedukowanych dla konkretnej sytuacji)
   */
   template< typename U > 
   static T* objectConstruction();
};


//
template< typename T, typename ID_KEY > T* Factory< T, ID_KEY >
::create( const ID_KEY& id ) const 
{
  Data::const_iterator  foundCreationMethod = fnPtrsCollection_.find( id );
  if( foundCreationMethod != fnPtrsCollection_.end() ) {
    ConstrFnPtr  createObjFunction = foundCreationMethod->second;
    return ( *createObjFunction )();
  } else
    throw UnvalidID( id );
}

//
template< typename T, typename ID_KEY > template< typename U > 
void Factory< T, ID_KEY >
::reg( const ID_KEY& id )
{
  if( fnPtrsCollection_.count( id ) != 0 )
    throw DoubledID( id );

  ConstrFnPtr  builderMethod = objectConstruction< U >;

  fnPtrsCollection_.insert( Data::value_type( id, builderMethod ) );
}
   
//
template< typename T, typename ID_KEY > bool Factory< T, ID_KEY >
::unreg( const ID_KEY& id )
{
  Itor  functionToUnReg = fnPtrsCollection_.find( id );
  if( functionToUnReg != fnPtrsCollection_.end() ) {
    fnPtrsCollection_.erase( functionToUnReg );
    return true;
  } else
    return false;
}

//
template< typename T, typename ID_KEY > inline
Factory< T, ID_KEY >::IDsConstItor Factory< T, ID_KEY >
::begin()
{
  return IDsConstItor( fnPtrsCollection_.begin() );
}

//
template< typename T, typename ID_KEY > inline
Factory< T, ID_KEY >::IDsConstItor Factory< T, ID_KEY >
::end()
{
  return IDsConstItor( fnPtrsCollection_.end() );
}

//
template< typename T, typename ID_KEY > template< typename U >
T* Factory< T, ID_KEY >
::objectConstruction()
{
  return new U;
}

} // namespace Utilities
} // namespace Gcad

#endif
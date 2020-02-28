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

#ifndef _GCAD_VALUEPTR_H_
#define _GCAD_VALUEPTR_H_

#include "GcadAssertion.h"
#include <utility>

namespace Gcad {
namespace Utilities {

/**
  @brief
    Cechy charakerystyczne szablonu ValuePtr
*/
template< typename T >
class ValuePtrTriats {
 public:
   /**
     @brief 
       Domyslne zachowanie kopiowania zagregowanego obiektu (dana 
       skladowa ptr_ zawarta w instancji klasy low::ValuePtr) przy uzyciu
       konstruktora kopiujacego parametryzowanego typu T

     @remark
       W celu przedefiniowania tego domyslnego zachowania kopiowania
       nalezy utworzyc specjalizacje szablonu cech charakterystycznych,
       zgodna z pozadana semantyka kopiowania
   */
   static T* clone( const T* clonedInstance ) 
   {
     // Test poprawnosci wskaznika clonedInstance jest wykonywany
     // wewnatrz skonkreyzowanej metody createFrom( const U* ), 
     // zgniezdzonej w klasie ValuePtr
     return new T( *clonedInstance ); 
   }
};

/**
  @brief
    Szablon implementacji "inteligentnego wskaznika", ktorego zachowanie
    podczas kopiowania jest zgodne z semantyka kopiowania wartosci    

  @remark
    Informacje dotyczace ponizszego rozwiazania problemu projektowego 
    "inteligentnego wskaznika" znajduja sie w ksiazce "More Exceptional C++"
    Herb'a Sutter'a

  @code
    #include "GcadValuePtr.h"
    #include <iostream>

    using namespace Gcad::Utilities;
    using namespace std;

    class B {
     public:
       virtual ~B() {}
       
       virtual B*           clone() const { return new B; }
       virtual const char*  talk()  const { return "B::talk()"; }
    };

    class D : public B {
     public:
       virtual D*           clone() const { return new D; }
       virtual const char*  talk()  const { return "D::talk()"; }
    };

    // Wymagana specjalizacja definujaca prawidlowy sposob kopiowania
    // za posrednictwem instancji klasy szablonowej ValuePtr< B >.
    // Szablon klasy ValuePtr< D > nie wymaga specjalizacji cech
    // charakterystycznych, poniewaz domyslna ich implementacja jest
    // tozsama z jego hipotetyczna wersja specjalizowana

    template<>
    class ValuePtrTriats< B > {
     public:
       static B* clone( const B* p ) { return p->clone(); }
    };

    struct MemberPtrAgregate {
      MemberPtrAgregate()
        : b_( new B ),
          d1_( new D ),
          d2_( new D )
      {}

      ValuePtr< B >  b_;
      ValuePtr< B >  d1_;
      ValuePtr< D >  d2_;
    };

    int main()
    {
      MemberPtrAgregate  a;
      MemberPtrAgregate  b;

      // Wykonanie automatycznego kopiowania skladowych
      b = a;
    
      // Sprawdzenie poprawnosci wykonanej kopii
      cout << 
        "Wskaznik do B, wskazujacy na instancje B: " << b.b_->talk() << endl <<
        "Wskaznik do B, wskazujacy na instancje D: " << b.d1_->talk() << endl <<
        "Wskaznik do D, wskazujacy na instancje D: " << b.d2_->talk() << endl;        

      // Inteligentne wskazniki sa z natury autonomiczne. W czasie destrukcji
      // podejmuja one stosowne dzialania, polegajace na zwolnieniu zasobow
      // za ktore sa odpowiedzialne
    }
  @endcode
*/
template< typename T >
class ValuePtr {
 public:
   // Pomijajac deklaracje przyjazni z typami konkretnymi ValuePtr,
   // nie bylby mozliwy dostep do ich wartosci prywatnej ptr_, poniewaz
   // klasy konkretne utworzone z szablonow nie sa powiazane zadnymi
   // zwiazkami przyjazni. Sa one jednak wymagane do poprawnego dzialania
   // konstruktorow kopiujacych, oraz operatorow przypisania
 
   template< typename U > friend class ValuePtr;

   /**
     @brief Definicja konstruktora
   */
   explicit ValuePtr( T* allocatedInstance = 0 )
     : ptr_( allocatedInstance ) {}

   /**
     @brief Destruktor stosujacy polityke automatycznego zwolnienia zasobu
   */
   ~ValuePtr() { delete ptr_; }

   /**
     @brief 
       Wymagany konstruktor kopiujacy zgodny z wymaganiami utworzenia 
       osobnej kopii agregowanej instancji      
   */
   ValuePtr( const ValuePtr& rhs )
     : ptr_( createFrom( rhs.ptr_ ) )
   {}

   /**
     @brief
       Operator przypisania kopiujacego zgodny z wymaganiami projektowymi
       stawianymi implementowanej klasie
   */
   ValuePtr& operator =( const ValuePtr& rhs ) {
     ValuePtr temp( rhs );
     swap( temp );
     return *this;
   }

   /**
     @brief 
       Szablonowa wersja konstruktora kopiujacego
     
     @remark
       Potrzeba przyjazni z odmiennymi warjacjami szablonu zostaje
       uwidaczniona w tej metodzie konstruktora kopiujacego. W przypadku
       braku deklaracji przyjazni, kompilator protestowalby widzac uzycie
       skladnika rhs.ptr_, tym samym oznajmajac niemoznosc dostepu do 
       prywatnej implementacji szablonu ValuePtr< U > z zakresu szablonu
       ValuePtr
   */
   template< typename U >
   ValuePtr( const ValuePtr< U >& rhs )
     : ptr_( createFrom( rhs.ptr_ ) )
   {}

   /**
     @brief 
       Szablonowa wersja operatora przypisania, wykorzystujaca idiom
       konstrukcji semantycznej bezpiecznej pod wzgledem obslugi wyjatkow
   */
   template< typename U >
   ValuePtr& operator =( const ValuePtr< U >& rhs ) {
     ValuePtr temp( rhs );
     swap( temp );
     return *this;
   }

   /**
     @brief Dereferencja inteligentnego wskaznika
   */
   T& operator *() const { return *ptr_; }

   /**
     @brief Akcesor udostepniajacy agregowana strukture
   */
   T* operator ->() const { 
     assertion( ptr_ != 0, "Proba wyluskania pustego wskaznika!" );
     return ptr_; 
   }

   /**
     @brief 
       Metoda implementujaca "roszade" wewnetrznych wskaznikow. 
       
       Poniewaz operacje wykonywane za posrednictwem wskaznikow sa opatrzone 
       specyfikatorem throw(), realizacja metody podtrzymuje zasade idiomu
       kopiowania bezpiecznego pod wzgledem obslugi sytuacji wyjatkowych
   */
   void swap( ValuePtr& valuePtr ) {
     std::swap( ptr_, valuePtr.ptr_ ); 
   }

   /**
     @brief Odpowiedz na pytanie: "Czy wskaznik aktualnie zawiera adres?"
   */
   bool empty() const { return ptr_ == 0; }

 private:
   /**
     @brief
       Hermetyzacja dzialania tworzenia obiektu, wykorzystujaca 
       metode zawarta w szablonie cech charakterystycznych
   */
   template< typename U >
   T* createFrom( const U* clonedInstance ) const 
   {
     return clonedInstance ? ValuePtrTriats< U >::clone( clonedInstance ) : 0;
   }

 private:
   T*  ptr_; /**< Adres zasobu, za ktory odpowiedzialny jest agregat */
};

} // namespace Utilities
} // namespace Gcad

#endif

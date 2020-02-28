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

#ifndef _GCAD_ISDERIVEDFROM_H_
#define _GCAD_ISDERIVEDFROM_H_

namespace Gcad {
namespace Utilities {

/**
  @brief
    Szablon swiadczacy uslugi - ktorych rezultat jest obliczany 
    na etapie kompilacji - zwiazane z okresleniem powiazania
    typow DERIVED, oraz BASE

  @remark
    Informacje dotyczace sposobu dzialania, oraz przykladowe
    wykorzystanie szablonu mozna znalezc w "More Exceptional C++"
    Herb'a Sutter'a

  @code
    #include "GcadIsDerivedFrom.h"
    #include <iostream>

    using namespace Gcad::Utilities;
    using namespace std;

    class Base {};
    class Derived : public Base {};

    // Automatyczne wykonanie testu za posrednictwem proby 
    // konkretyzacji konstruktora klasy IsDerivedFrom
    class Test : IsDerivedFrom< Derived, Base > {};

    int main()
    {
      // Kompilator tworzy automatycznie domyslny konstruktor
      // Test::Test(), ktory wywoluje konstruktor - czesci bazowej -
      // klasy IsDerivedForm< Derived, Base > potwierdzajacy relacje
      // dziedziczenia klasy Derived po Base (nie wystepuje blad
      // w czasie kompilacji).
      
      Test testConstraintCtor;

      // Wykonanie testu, poprzez sprawdzenie wyliczonej wartosci
      // reprezentowanej przez stala wyliczeniowa
      
      const bool test = IsDerivedFrom< Base, int >::IsIt;
      cout << "Base " << ( test ? "" : "nie" ) << " dziedziczy po int" << endl;
    }
  @endcode
*/
template< typename DERIVED, typename BASE >
class IsDerivedFrom {
 private:
   /**
     @brief
       Klasa wykorzystywana do uzyskania, okreslenia wartosci 
       zwroconej przez operator sizeof
   */
   class No {}; 
   
   /**
     @brief
       Rezultatem dzialania operatora sizeof, wykorzystujacego argument
       w postaci klasy Yes, jest wartosc rozna od sizeof( No )
   */
   class Yes { No no[ 2 ]; };

   /**
     @brief
       Nie zaimplementowane deklaracje funkcji, wykorzystywane jedynie 
       w celu okreslenia wartosci zwroconej, dostepnej poprzez determinacje
       konkretnej procedury, wykonanej podczas etapu rozstrzygania 
       przeladowania nazw funkcji
   */
   static Yes  Test( BASE* );
   static No   Test( ... );

   /**
     @brief
       Funkcja ograniczen definiujaca dzialanie majace na celu okreslenie
       relacji klas DERIVED, oraz BASE
   */
   static void Constraints() { 
     // Proba rzutowania adresu typu DERIVED na typ BASE. W sytuacji, gdy
     // DERIVED nie jest podtypem klasy BASE, mechanizm kontroli typow
     // oznajmi ich niezgodnosc
     BASE* base = static_cast< DERIVED* >( 0 );
     
     // Instrukcja usuwajaca ostrzezenie kompilatora o nieuzywanej zmiennej base
     base = 0; 
   }

 public:
   enum { 
     /**
       @brief
         Wartosc okreslona na etapie kompilacji, opisujaca relacje 
         zawarta miedzy typami DERIVED, oraz BASE
       
       @remark
         W sytuacji braku powiazania miedzy klasami, okreslonego 
         dziedziczeniem, wartosc IsIt jest rowna zeru. W przeciwnym
         wypadku jest tozsama jednosci
     */
     IsIt = sizeof( Test( static_cast< DERIVED* >( 0 ) ) ) == sizeof( Yes ) 
   };

   /**
     @brief
       Definicja mechanizmu wykorzystywanego przez szablony stawiajace 
       wymagania odnosnie rodzaju typu ich parametrow
   */
   IsDerivedFrom() {
     // Proba konkretyzacji funkcji ograniczen
     void ( *fn )() = Constraints;
   }
};

} // namespace Utilities
} // namespace Gcad

#endif
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

#ifndef _GCAD_GENERICKEYMAPCONSTITOR_H_
#define _GCAD_GENERICKEYMAPCONSTITOR_H_

namespace Gcad {
namespace Utilities {

/**
  @brief
    Szablon automatyzujacy usluge udostepnienia realizacji iteratora
    kolekcji typu std::map, do iteracji poprzez elementy kluczy
    
    Wzorzec ten jest zwykle konkretyzowany w klasach umozliwiajacych
    odwolywanie do kluczy kontenera. Typ iteratora - w wiekszosci
    przypadkow, zagniezdzony w klasie udostepniajacej agregowane elementy -
    wykonujacy trawersacje po wartosciach kluczy jest tworzony poprzez
    publiczne dziedziczenie implementacji kontenera klasy szablonowej 
    rodzaju std::map (kolekcja przewaznie jest agregowana przez typ 
    udostepniajacy iterator do wartosci kluczy). Podtyp jest jedynie 
    zobowiazany do utworzenia wlasciwego konstruktora, przekazujacego 
    odpowiedzialnosc do klasy bazowej

  @remark
    Mozna zauwazyc, iz konstrukcja instancji typu nie jest mozliwa,
    poniewaz konstruktory umieszczone sa w sekcji prywatnej. W klasie
    nie znajduja sie rowniez zadne metody wirtualne. Powinien byc to 
    sygnal dla klientow kozystajacych z dostarczonych wlasciwosci,
    aby nie wykorzystywac podtypow poprzez ich iterfejs bazowy.
    Zadaniem klasy jest udostepnienie implementacji, oraz zautomatyzowanie
    redundandnych czynnosci typow definiujacych uslugi zdefiniowane
    w szablonie GenericKeyMapConstItor. Utozsamienie typu z interfejstem
    (klasa protokolu, zeby byc scislym) jest niepoprawne

  @see
    Gcad::Utilities::AssociativeManager::IDsConstItor
*/
template< typename MAP >
class GenericKeyMapConstItor {
 public:
   typedef typename MAP::const_iterator   ItorType;
   typedef typename MAP::key_type         KeyType;

   GenericKeyMapConstItor& operator ++() { 
     ++itor_;
     return *this;
   }

   GenericKeyMapConstItor operator ++( int ) { 
     GenericKeyMapConstItor  temp( itor_ );
     ++*this;
     return temp;
   }

   GenericKeyMapConstItor& operator --() { 
     --itor_;
     return *this;
   }

   GenericKeyMapConstItor operator --( int ) { 
     GenericKeyMapConstItor  temp( itor_ );
     --*this;
     return temp;
   }

   const KeyType& operator *()  const { return itor_->first; }
   const KeyType* operator ->() const { return &itor_->first; }

   bool operator ==( const GenericKeyMapConstItor& rhs ) const {
     return itor_ == rhs.itor_;
   }

   bool operator !=( const GenericKeyMapConstItor& rhs ) const {
     return !( *this == rhs ); 
   }

 protected:
   explicit GenericKeyMapConstItor( ItorType itor )
     : itor_( itor )
   {}
  
 private:
   ItorType  itor_;
};

} // namespace Utilities
} // namespace Gcad

#endif
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

#ifndef _GCAD_DRIVEELEMENTSENUMERATOR_H_
#define _GCAD_DRIVEELEMENTSENUMERATOR_H_

#include "GcadBase.h"
#include <memory>
#include <string>

namespace Gcad {
namespace Platform {

/**
  @brief
    Klasa hermetyzujaca zachowanie wyliczenia nazw plikow
    o zadanym rozszerzeniu znajdujacych sie w okreslonym 
    przez klienta katalogu. Dzieki posredniosci wprowadzonej
    warstwa abstrakcji, implementacja nie jest zwiazana
    bezposrednio z danya platforma systemowa
*/
class GCAD_EXPORT DriveElementsEnumerator {
 public:
   virtual ~DriveElementsEnumerator();

   /**
     @brief
       Iterator wykorzystywany do uzyskania nazw elementow wchodzacych
       w sklad reprezentacji katalogu, dla ktorego aktualnie pracuje

     @remark
       Podczas zmiany pierwotnego katalogu, przez obiekt typu
       DriveElementsEnumerator, instancja iteratora jest zobowiazana
       wywolac metode moveToBegin(), aby uzyskac informacje o nowym
       korzeniu przeszukiwania katalogow. W przeciwnym wypadku, 
       iterator bedzie dzialal w obszarze katalogu pierwotnie ustalonego
       (podczas konstrukcji jednostki iteratorowej)
   */
   class GCAD_EXPORT Itor {
    public:
      virtual ~Itor();

      /**
        @brief
          Danie odpowiedzi na pytanie: czy dany iterator jest ustawiony
          na elemencie struktury bedacej katalogiem?
      */
      virtual bool isDirectory() const = 0;

      /**
        @brief
          Pobranie nazwy elementu aktualnie wskazywanego przez iterator,
          bedacego jednostka struktury hierarchii drzewa katalogow/plikow
      */
      virtual std::string getElementName() const = 0;

      /**
        @brief 
          Przesuniecie iteratora na kolejny element hierarchii elementow
          wyszczegolnionego katalogu
      */
      virtual bool moveToNextElement() = 0;

      /**
        @brief
          Z pomoca wykorzystania metody mozliwe jest okreslenie warunku
          zakonczenia iteracji. Wartosc zwrocona typu bool informuje 
          o poprawnosci elementu wskazywanego przez iterator. W przypadku,
          gdy rezultatem dzialania funkcji jest wartosc falsz, jednoznacznie
          determinuje to fakt zakonczenia trawersacji struktury katalogu
      */
      virtual bool isValid() const = 0;

      /**
        @brief 
          Ustawienie iteratora na wezel poczatkowy hierarchii struktury
          organizacyjnej plikow/katalogow. Nadrzedny korzen jest specyfikowany 
          przez obiekt typu DriveElementsEnumerator, z ktorym zwiazany jest 
          iterator (obiekt typu DriveElementsEnumerator jest jego tworca)

        @remark
          Wywolanie metody spowoduje blad, gdy obiekt z ktorym zwiazany 
          jest iterator (typu DriveElementsEnumerator) zostal przed jej
          wykonaniem zniszczony
      */
      virtual void moveToBegin() = 0;
   };

   /**
     @brief
       Skorzystanie z wzorca metody klonujacej - wirtualnego konstruktora
   */
   virtual DriveElementsEnumerator* clone() const = 0;

   /**
     @brief Typ zapewniajacy poprawna dezaktualizacje obiektu iteratora
   */
   typedef std::auto_ptr<Itor>  ItorAutoPtr;

   /**
     @brief 
       Metoda fabryczna zwracajaca iterator ustawiony na poczatkowym
       wezle hierarchii katalowow/plikow. Do czasu zniszczenia iteratora
       jest on na stale zwiazany ze swoim stworca
   */
   virtual ItorAutoPtr getItor() const = 0;

        
   /**
     @brief
       Ustalenie katalogu przeszukiwania
       
      @remark
        Zewnetrzne iteratory, ktorymi posluguje sie uzytkownik nie sa
        automatycznie aktualizowane. Aby otrzymaly informacje o ewentualnej
        zmianie korzenia katalogu, musza otrzymac zadanie moveToBegin()
   */
   virtual void setDirectory(const std::string& directory) = 0;
};

} // namespace Platform
} // namespace Gcad

#endif

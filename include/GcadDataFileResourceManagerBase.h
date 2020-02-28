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

#ifndef _GCAD_DATAFILERESOURCEMANAGERBASE_H_
#define _GCAD_DATAFILERESOURCEMANAGERBASE_H_

#include "GcadBase.h"
#include "GcadException.h"
#include <string>

namespace Gcad {
namespace Platform {

/**
  @brief
    Wywyzszona wspoldzielona funkcjonalnosc menadzera zapobiegajaca
    redundantnym definicjom implikowanym przez konkretyzacje szablonu
    DataFileResourceManager. Umieszczajac ja wewnatrz typu szablonowego
    kazda realizacja konkretnego nowego typu niepotrzebnie powielalaby 
    identyczne definicje, oraz zachowania, co byloby powodem zbednej
    nadmiarowosci

  @remark
    Klasa stanowi jedynie baze, punkt wyjscia dla typow powstalych z szablonu
    DataFileResourceManager. Aby wyegzekwowac to ograniczenie na
    kliencie, w tym celu zdefiniowany zostal konstruktor domyslny 
    znajdujacy sie w zakresie dzialania specyfikatora protected, tym samym
    uniemozliwiajac tworzenie instancji typu DataFileResourceManagerBase
*/
class DataFileResourceManagerBase {
 public:
   /**
     @brief 
       Typ wyliczeniowy determinujacy priorytet zasobu. Jest to pierwsze
       kryterium (brane pod uwage w pierwszej kolejnosci), ktore jest
       uwzgledniane podczas segregowania (sortowania) zasobow agregowanych 
       w menadzeze. Drugim czynnikiem, ktory bezposrednio wplywa na miejsce 
       polozenia - tym samym waznosci zasobu - jest wartosc determinujaca 
       ostatni czas dostepu do danych agregatu
   */
   enum ResourcePriority {
     LOW_PRIORITY,
     MEDIUM_PRIORITY,
     HIGH_PRIORITY
   };

   /**
     @brief
       Niemoznosc odwzorowania identyfikatora na jednoznacznie zdefiniowany
       zasob jest sygnalizowana obiektem wyjatku typu ResourceFileIdAmbiguous.
       Do takiej sytuacji moze dojsc podczas pobierania zasobu, podajac
       w wywolaniu jedynie jego identyfikator, bedacy nazwa pliku
       zawierajacego pozadane dane. Poniewaz identyfikator okreslajacy
       zewnetrzne zrodlo danych moze byc reprezentowany w podrzewach 
       o korzeniach bedacych wezlami hierarchii katalogow, wiec mozliwosc 
       zaistnienia niejednoznacznosci jest prawdopodobna (rozne katalogi
       moga posiadac pliki o rownoznacznych nazwach - identyfikatorach -
       wiec wykonanie zadania pobrania zadanego zasobu nie moze zostac spelnione)
   */
   class GCAD_EXPORT ResourceFileIdAmbiguous : public Utilities::Exception {
     friend class DataFileResourceManagerBase;
     ResourceFileIdAmbiguous(const std::string& ambiguousId)
       : Utilities::Exception("Dwuznaczny identyfikator: " + ambiguousId)
     {}
   };

   /**
     @brief
       Klasa wyjatku powstalego w sytuacji niemoznosci odwzorwania
       identyfikatora na zagregowany zasob. Jezeli zadana nazwa 
       opisujaca pozadany zasob nie wspolgra z zawartoscia menadzera,
       oraz nie jest mozliwe dopasowanie - polegajace na transformacji
       identyfikatora z uzyciem danych ilustrujacych katalogi - majace 
       na celu poprawe tej sytuacji, wowczas jedynym prawidlowym 
       rozwiazaniem jest poinformowanie klienta o zajsciu blednego
       zdarzenia, poprzez wyrzucenie obiektu typu UnvalidIdException
   */
   class GCAD_EXPORT UnvalidIdException : public Utilities::Exception {
     friend class DataFileResourceManagerBase;
     UnvalidIdException(const std::string& unvalidId)
       : Utilities::Exception("Niepoprawny identyfikator: " + unvalidId)
     {}
   };

   /**
     @brief
       Podtypy klasy protokolu moga zostac zdefiniowane przez klienta 
       uzytkownika. Przekazujac podklase menadzerowi zasobow, mozemy
       zdefiniowac nowe strategie wykonywana podczas wczytywania 
       (leniwa inicjalizacja), oraz pozyskiwania zasobow (rzeczywista 
       praca odczytu danych z dysku - pliku zasobu)
   */
   class GCAD_EXPORT ResourceManage {
    public:
      virtual ~ResourceManage();

      /**
        @brief
          Metoda wywolywana przez menadzer zasobow w sytuacji pobierania
          danych z zewnetrznego zrodla
      */
      virtual void acquire(const std::string& directory,
                           const std::string& fileName) = 0;

      /**
        @brief
          Metoda wywolywana przez instancje menadzera zasobow podczas
          wykonywania pobrania metoda leniwej inicjalizacji
      */
      virtual void load(const std::string& directory,
                        const std::string& fileName) = 0;
      /**
        @brief
          Metoda wywolywana przez instancje menadzera zasobow podczas
          zwalniania pamieci zasobu, spowodowanej proba pozyskania
          noewgo zasobu (nie majacego odwzorowania w menadzerze) 
          w warunkach niedoboru pamieci
      */
      virtual void release(const std::string& directory,
                           const std::string& fileName) = 0;

      /**
        @brief
          Metoda wywolywana przez instancje menadzera zasobow w celu
          bezwzglednego usuniecia wspoldzielonego zasobu z wewnetrznej
          reprezentacji agregatu. Wszystkie zewnetrzne uchwyty zostaja
          momentalnie uniewaznione. Proba odwolania sie do zasobu 
          za posrednictwem ktoregos z nich bedzie zakonczona zaistnieniem
          bledu czasu wykonania
      */
      virtual void remove(const std::string& directory,
                          const std::string& fileName) = 0;
   };

   /**
     @brief
       Skorzystanie z wzorca NullObject, ktorego zastosowanie umozliwia
       bezproblemowa implementacje, pozbawiona sprawdzen warunkowych
       majacych na celu okreslenie poprawnosci wartosci wskaznika. 
       Ciala definicji zachowan obiektow wspierajacych semantyke 
       zapozyczona z wzorca projektowego nazwanego NullObject, nie wykonuja
       zadnych dzialan, stad brak jakichkolwiek towarzyszacych im instrukcji
   */
   class GCAD_EXPORT NullResourceManage : public ResourceManage {
    private:
     friend class DataFileResourceManagerBase;
     NullResourceManage() {}
    
    public:
      virtual void acquire(const std::string& directory,
                           const std::string& fileName);

      virtual void load(const std::string& directory,
                        const std::string& fileName);
     
      virtual void release(const std::string& directory,
                           const std::string& fileName);

      virtual void remove(const std::string& directory,
                          const std::string& fileName);
   };

   /**
     @brief
       Instancja typu realizujacego interfejs sluzy za kryterium
       dopasowywania przegladanych nazw plikow/katalogow do specyficznych
       wymagan podyktowanych przez menadzera
   */
   class GCAD_EXPORT ElementMatcher {
    public:
      virtual ~ElementMatcher();

      /**
        @brief
          Metoda definiuje zachowanie, ktore jest wykorzystywane
          przez instancje menadzera. W czasie pobierania kolejnych
          elementow struktury katalogow/plikow analizuje on na podstawie
          zdefiniowanego kryterium (metody) mozliwosc pobrania (wczytania)
          aktualnie sprawdzanego pliku zasobu

        @param element Nazwa pliku, badz katalogu przekazana przez menadzer,
          ktory wykonuje akcje przegladania struktury drzewa katalogow/plikow
      */
      virtual bool match(const std::string& element) const = 0;
   };

   /**
     @brief
       Skorzystanie z semantyki zawartej w granicach wzorca NullObject. 
       Podstawowe zachowanie instancji, polegajace na zwracaniu wartosci
       prawda w sytuacji wykonywania sprawdzenia, jest wykorzystywane
       domyslnie przez obiekt menadzera
   */
   class GCAD_EXPORT NullElementMatcher : public ElementMatcher {   
      friend class DataFileResourceManagerBase;   
    public:
      virtual bool match(const std::string& element) const;
   };

 protected:
   // Zablokowanie przed klientem mozliwosci tworzenia instancji
   DataFileResourceManagerBase() {}

   /**
     @brief 
       Umozliwienie tworzenia wyjatku w klasach pochodnych, ktory oznajmia
       wystapienie dwuznacznej sytuacji (dopasowania jednego identyfikatora
       do dwoch roznych posrednikow zasobow)
   */
   ResourceFileIdAmbiguous
   createResourceFileIdAmbiguousException(const std::string& ambiguousId) {
     return ResourceFileIdAmbiguous(ambiguousId);
   }

   /**
     @brief 
       Umozliwienie tworzenia wyjatku w klasach pochodnych, ktory oznajmia
       niemoznosc dopasowania zasobu na podstawie zadanego identyfikatora
   */
   UnvalidIdException
   createUnvalidIdException(const std::string& unvalidId) {
     return UnvalidIdException(unvalidId);
   }

   /**
     @brief
       Umozliwienie tworzenia instancji typu NullResourceManage przez 
       klasy dziedziczace po DataFileResourceManagerBase
   */
   NullResourceManage*
   createNullResourceManage() const { return new NullResourceManage(); }

   /**
     @brief
       Umozliwienie tworzenia instancji typu NullElementMatcher klasom pochodnym
   */
   NullElementMatcher*
   createNullElementMatcher() const { return new NullElementMatcher(); }
};

} // namespace Platform
} // namespace Gcad

#endif

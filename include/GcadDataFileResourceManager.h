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

#ifndef _GCAD_DATAFILERESOURCEMANAGER_H_
#define _GCAD_DATAFILERESOURCEMANAGER_H_

#include "GcadDataFileResourceManagerBase.h"
#include "GcadAssertion.h"
#include "GcadDriveElementsEnumerator.h"
#include "GcadException.h"
#include "GcadSymbolTST.h"
#include "GcadTimeInformation.h"
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <utility>

/** 
  @todo Tworzenie sciezek korzysta z stalej doslownej "\\". Dla innych 
    systemow (np. UNIX) oddzielenie nazwy katalogu wykonuje sie wartoscia
    "/". Rozwazyc utworzenie nowej klasy hermentyzujacej ta zmiennosc.
*/

namespace Gcad {
namespace Platform {

/**
  @brief
    Wyspecjalizowana klasa menadzera znajdujaca zastosowanie w zadaniach
    polegajacych na zaglowaniu duzymi porcjami danych podczas dzialania 
    programu. Do glownych jej zalet naleza:
      - automatyczne wczytywanie danych na zadanie klienta (leniwa
        inicjalizacja) zrealizowania z pomoca posrednikow
      - zautomatyzowanie procesu wyszukiwania zrodel danych (podanie
        nazwy katalogu, od ktorego rozpoczac ma sie przeszukiwanie
        zasobow spelniajacych zadane kryterium)

  @param
    RESOURCE Typ klasy determinujacej wspoldzielony zasob. Jej interfejs 
    musi byc wyposazony w zachowania okreslone przez klasy wspolpracujace
    z klasa menadzera zasobow DataFileResourceManager. Pozadane sygnatury 
    maja z gory zadana postac, oraz reprezentuja zachowanie rownozedne 
    z podanymi ponizej wytycznymi:
      - size_t getByteSize() const - zwracajaca wlasciwa wielkosc calej
        struktury tworzacej zasob (suma jej skladowych bezposrednich,
        oraz posrednio dostepnych przez wskazniki i referencje)
      - RESOURCE(std::basic_ifstream<FILE_FORMAT>&) - konstruktor 
        jednoargumentowy, wywolywany w trakcie pozyskiwania danych.
        Jego glownym zadaniem (obowiazkiem) jest wczytanie danych zawartych 
        w pliku zasobu agregujacym obiekt typu parametryzowanego RESOURCE
  
  @param
    FILE_FORMAT Wewnetrzny format danych pliku zasobu moze byc realizowany
    w dwojaki sposob. Przewaznie jest to reprezentacja, ktora definiuje
    realizacje jednego znaku w formie 8 bitow opisywanych przez typ char
    (ANSI). Drugie spektrum, to kodowanie UNICODE, ktore przydziela
    pamiec wielkosci dwoch bajtow (16 bitow) na jeden znak (typ wchar_t)

  @todo 
    Zamienic zmienne istream na ogolniejsza wersje basic_ifstream. 
    Pliki rowniez chyba moga posiadac reprezentacje ANSI, lub UNICODE. 
    Jesli tak, to zamiana wszystkich obiektow typu string odnoszacych 
    sie do nazw plikow - a tych troche jest :( - rowniez pozadana
*/
template<typename RESOURCE,
         typename FILE_FORMAT = char>
class DataFileResourceManager : public DataFileResourceManagerBase {
 private:
   class SharedResources;
   friend class SharedResources;
 
 public:   
   class Handle;
   friend class Handle;

 private:
   //! @todo wszystkie odwolania ifstream -> InputFileStream
   //typename std::basic_ifstream<FILE_FORMAT, ??>  InputFileStream;
   
   /**
     @brief
      Typ odwzorowujacy sciezke dostepu zasobu, na jednoznacznie
      okreslona wartosc go identyfikujaca
   */
   typedef Utilities::SymbolTST<FILE_FORMAT>  PathToTSTHandle;
   typedef typename PathToTSTHandle::Handle   TSTHandle;
    
   /**
     @brief
       Wewnetrznia reprezentacja wspoldzielonego zasobu agregowanego
       w menadzerze. Po utworzeniu instancji prawo wlasnosci otrzymuje
       menadzer, ktory jest odpowiedzialny za jego istnienie
     
     @remark
       Istnieje bezwzgledna restrykcja dotyczaca zasobow mogacych 
       zostac agregatami menadzera: sa one wspoldzielone, wiec nie
       moga one posiadac funkcji modyfikatorow, zmieniajacych stan
       wewnetrzny wspoldzielonego obiektu
   */
   class SharedResource {
    public:
      /**
        @brief
          Klasa wyjatku powstalego w wyniku bledu w trakcie proby
          otworzenia strumienia wejscia majacej na celu pozyskanie
          zewnetrznie zaalokowanych danych
      */
      class FailOnResourceAcquireException : public Utilities::Exception {
        friend class SharedResource;
        FailOnResourceAcquireException(const std::string& resourceId)
          : Utilities::Exception( "Nasapil blad podczas pozyskiwania "
            "zasobu o identyfikatorze: " + resourceId )
        {}
      };

      /**
        @brief
          Konstruktor stosujacy zasade leniwej inicjalizacji zasobu.
          W czasie proby odwolania sie do wspoldzielonego zasobu, ktory
          nie znajduje sie aktualnie w pamieci, automatycznie zostanie
          wykonana praca polegajaca na wczytaniu danych ze zrodla okreslonego
          posrednio przez parametry konstruktora - bezposrednio przez uchwyt
          mogacy zostac przetlumaczony na jednoznaczna sciezke dostepu zasobu
      */
      SharedResource(DataFileResourceManager&  owner,
                     const std::string&        dataFileName,
                     const std::string&        dataFilePath);

      /**
        @brief
          Wymagany przez standardowy kontener <i>set</i> konstruktor kopiujacy.
          Kompilator nie moze przeprowadzic automatycznej generacji, poniewaz
          jedna ze skladowych jest typu std::auto_ptr. Semantyka kopiowania
          tego rodzaju instancji jest rozna od konwencjonalnego, przyjetego
          idiomu kopiowania pobierajacego argument do stalej referencji
      */
      SharedResource(const SharedResource& rhs)
        : owner_(rhs.owner_)
        , handle_(rhs.handle_)
        , priority_(rhs.priority_)
        , resource_(rhs.resource_.release())
        , lastAccess_(rhs.lastAccess_)
        , referenceCounter_(rhs.referenceCounter_)
      {
      }      

      /**
        @brief 
          Zadanie pozyskania zasobu z zewnetrznego zrodla. Operacja wykonuje
          wlasciwe pobranie danych z pliku dyskowego
      */
      void acquire() const;

      /**
        @brief 
          Zadanie zwolnienia pamieci przydzielonej wspoldzielonemu zasobowi
      */
      void release() const;

      /**
        @brief
          Inkrementacja licznika odwolan do wspoldzielonego zasobu
      */
      void increaseRef() const;
      
      /**
        @brief
          Dekrementacja licznika odwolan do wspoldzielonego zasobu
      */
      void decreaseRef() const;

      /**
        @brief
          Ustalenie priorytetu zasobu
          
          Poniewaz wartosc priorytetu wplywa na wzgledne polozenie 
          wewnatrz posortowanej struktury, dlatego w celu zachowania 
          jej organizacji przeprowadzane sa operacje zapewniajace 
          spelnienie kryterium zdefiniowanego przez wewnetrzna strukture
          organizacyjna menadzera
      */
      void setPriority(ResourcePriority priority) const;
      
      /**
        @brief
          Udostepnienie informacji dotyczacej priorytetu zasobu
      */
      ResourcePriority getPriority() const;
      
      /**
        @brief
          Rezultatem dzialania metody jest wartosc determinujaca 
          dokladna ilosc zajmowanej pamieci przez zasob. Na jej 
          podstawie menadzer potrafi okreslic wielkosc wolnej 
          pamieci, ktora pozostaje do jego dyspozycji (odnotowuje
          ta informacje w swojej wewnetrznej strukturze)

        @remark
          Metoda korzysta z funkcjonalnosci zdefiniowanej przez klienta
          w celu okreslenia dokladnej wielkosci zasobu
      */
      size_t getByteSize() const;

      /**
        @brief
          Zwrocenie informacji okreslajacej przetrzymywanie przez
          klienta odwolan (uchwytow) do wspoldzielonego zasobu
      */
      bool isShared() const;

      /**
        @brief
          Kryterium sortowania wykorzystywane przez menadzer. Instancje
          reprezentujace wspoldzielone zasoby sa sortowane w pierwszej
          kolejnosci wedlug priorytetow, a w przypadku gdy obie strony
          relacji maja rowny priorytet - na podstawie wartosci okreslajacej
          czas ostatniego dostepu do zasobu
      */
      bool operator <(const SharedResource& rhs) const;

      /**
        @brief
          Udostepnienie rzeczywistego zasobu
      */
      const RESOURCE& getResource() const;

      /**
        @brief
          Udostepnienie wartosci uchwytu, na podstawie ktorego istnieje
          mozliwosc pozyskania sciezki dostepu do przynaleznego zasobu
      */
      TSTHandle getTstHandle() const;

    private:
      /**
        @brief
          Uzyskanie informacji na temat aktualnego stanu rzeczywistego 
          zasobu, zwiazanego z jego aktualnym istnieniem (odwzorowaniem)
          w pamieci ogolnego dostepu
      */
      bool isLoaded() const;

      /**
        @brief
          Metoda wykonujaca wczytanie zasobu z zewnetrznego zrodla. Jest ona
          uzywana w celach pomocniczych, podczas uzyskiwania dostepu do zasobu
          (pobieranie uchwytu), ktory mogl zostac zwolniony przez menadzera
      */
      void update() const;

    private:
      typedef unsigned int             RefCounter;
      typedef std::auto_ptr<RESOURCE>  ResourceAutoPtr;

      DataFileResourceManager&  owner_;         /**< Wlasciciel zasobu */
      TSTHandle                 handle_;        /**< Uchwyt okreslajacy zrodlo */
      mutable ResourcePriority  priority_;      /**< Priorytet zasobu */
      mutable ResourceAutoPtr   resource_;      /**< Odwolanie do zasobu */
      mutable size_t            lastAccess_;    /**< Czas ostatniego dostepu */
      mutable RefCounter        referenceCounter_; /**< Licznik odwolan zasobu */
   };

 public:  
   /**
     @brief
       Klasa uchwytu posredniczacego w dostepie do zasobu. Bedac w jego
       posiadaniu klient ma mozliwosc odwolywania sie do wspoldzielonej
       instancji obiektu

       Skorzystanie z metody przechowywania referencji w formie zwyklej
       wartosci liczbowej (uchwyt struktury TST) niesie ze soba istotna
       korzysc. Wspoldzielone zasoby, reprezentowane przez swych posrednikow,
       zawarte wewnatrz struktury menadzera moga byc dowolnie przemieszczane.
       Agregat odpowiedzialny za zarzadzanie zasobami uzyskuje tym samym
       duza elastycznosc w przeprowadzaniu specjalistycznych dzialan 
       scisle z nim sprzezonych. Poslugujac sie odwolaniami w postaci
       wskaznikow, lub referencji, kazda zmiana w strukturze zasobow
       przeprowadzona przez menadzer wiazalaby sie z uniewaznieniem
       uchwytow zewnetrznych, posiadanych przez klienta. Dzieki 
       wykorzystaniu rozwiazania opartego na wartosciach-uchwytach problem
       dezaktywacji odwolan staje sie nieaktualny
   */
   class Handle {
    private:
      // Konstruktor klasy znajduje sie w sekcji prywatnej. Deklaracja
      // przyjazni daje mozliwosc instancji menadzera tworzenie 
      // obiektow uchwytow, udostepnianych nastepnie klientom. Uzytkownicy
      // korzystajacy z obiektow uchwytow nie posiadaja mozliwosci 
      // bezposredniego tworzenia instancji
      friend class DataFileResourceManager;
     
      /**
        @brief
          Obiekty zasobow sa konstruowane przez menadzer-fabryke. Klient
          nie jest upowazniony do ich tworzenia, dlatego konstruktor
          zawarty zostal w przestrzeni opatrzonej specyfikatorem private.
          Domyslnie wartosc zasobu determinujaca jego priorytet jest 
          ustalona na srednim poziomie. Dzieki udostepnieniu funkcji 
          modyfikujacych, klient moze w latwy sposob zamienic efekt tego
          zachowania, tym samym ustalajac pozadany priorytet
      */
      Handle(DataFileResourceManager&  owner,
             TSTHandle                 handle);

      /**
        @brief
          Pomocnicza funkcja zwracajaca odpowiadajacy wartosci
          uchwytu TST wspoldzielony zasob

        @remark
          Nalezy zauwazyc, iz metoda zwraca referencje do stalego
          obiektu, stad tez w klasie SharedResource zaistniala potrzeba
          opatrzenia kilku skladowych modyfikatorem mutable, oraz 
          dodania kwalifikatora const do kilku funkcji skladowych
      */
      const SharedResource& dereferenceResource() const;
  
    public:
      /**
        @brief
          Konstruktor kopiujacy umozliwia poprawne kopiowanie obiektow
          uchwytow (standardowa realizacja konstruktora wykonana przez
          kompilator nie bylaby odpowiednia)
      */
      Handle(const Handle& handle)
        : owner_(handle.owner_)
        , handle_(handle.handle_)
      {
        dereferenceResource().increaseRef();
      }
      
      ~Handle() {
        dereferenceResource().decreaseRef();
      }      
      
      Handle operator =(const& rhs) {
        dereferenceResource().decreaseRef();
        handle_ = rhs.handle_;
        dereferenceResource().increaseRef();
      }      

      void setPriority(ResourcePriority priority) {
        dereferenceResource().setPriority(priority);
      }

      ResourcePriority getPriority() const {
        return dereferenceResource().getPriority();
      }      

      const RESOURCE& operator *() const { 
        return dereferenceResource().getResource(); 
      }

      const RESOURCE* operator ->() const { 
        return &dereferenceResource().getResource(); 
      }

    private:
      // Obiekt uchwytu (wartosc handle_) wprowadza potrzebna warstwe 
      // abstrakcji. Rozwiazanie to umozliwia menadzerowi wykonywanie
      // wewnetrznych operacji, nie majacych oddzwieku w zewnetrznych
      // referencjach (uchwytach Handle). Uzyskana elastycznosc jest
      // podstawa koncepcji, na ktorej opiera sie zarzadca

      DataFileResourceManager&  owner_;
      TSTHandle                 handle_;                                               
   };

 public:
   /**
     @brief
       Konstruktor menadzera pobierajacy parametry okreslajace:
         - rodzaj plikow zasobow determinowany przez rozszerzenie
         - interfejs oddzielajacy specyficzne dzialania przeprowadzane
           przez system plikow
     
     @remark
       Domyslna wartoscia determinujaca maksymalny rozmiar pamieci
       mozliwy do wykorzystania jest jeden megabajt
   */
   DataFileResourceManager(DriveElementsEnumerator* fileEnumerator,
                           TimeInformation*         timer);

   /**
     @brief
       Zwrocenie informacji oznajmujacej ilosc zasobow znajdujacych
       sie pod kontrola zarzadcy. Zasoby te nie koniecznie musza aktualnie
       znajdowac sie w pamieci, dokladniej - wartosc rezultatu dzialania 
       funkcji determinuje sume wszystkich obiektow posrednikow
   */
   size_t resourcesCount() const { return resources_.size(); }

   /**
     @brief
       Modyfikator obiektu typu ResourceManage sprzezonego z menadzerem. 
       Aktualna instancja znajdujaca odwzorowanie wewnatrz obiektu zarzadcy
       zasobow zostaje automatycznie zwolniona podczas wykonania funkcji
   */
   void setResourceManage(ResourceManage* resourceManage);

   /**
     @brief
       Modyfikator typu ResourceManage. Aktualna instancja znajdujaca
       odwzorowanie w menadzerze zostaje zwolniona podczas wykonania funkcji
   */
   void setElementMatcher(ElementMatcher* elementMatcher);

   /**
     @brief
       Ustalenie maksymalnej granicy wielkosci obszaru pamieci 
       udostepnionej zasobom danego typu RESOURCE
   */
   void setMemoryMaximum(size_t memorySize);

   /**
     @brief
       Pobranie wartosci determinujacej aktualne maksimum obszaru pamieci
       mozliwego do wykorzystania przez menadzer zasobow
   */
   size_t getMemoryMaximum() const;

   /**
     @brief
       Przekazanie informacji sciezki poszukiwania ewentualnych plikow 
       zawierajacych dane zasobow
   */
   void setResourcesSearchDir(const std::string& path);
   
   /**
     @brief
       Dezaktualizacja sciezki poszukiwania zasobow
   */
   bool removeResourcesSearchDir(const std::string& path);

   /**
     @brief
       Wyszukanie, oraz ewentualne pozyskanie zasobu na podstawie
       determinujacego go identyfikatora w formie nazwy pliku. W zadaniu
       wykonywane jest dopasowywanie identyfikatora do roznych mozliwych
       postaci - ogolnie zasoby sa identyfikowane przez nazwe pliku
       poprzedzona przez nazwe katalogu, w ktorym sa przechowywane.
       Wynika z tego fakt, iz w celu prawidlowego dopasowania, menadzer
       przeprowadza szereg sprawdzen majacych na celu ustalenie precyzyjnego,
       jednoznacznego sprzezenia identyfikatora z zasobem (kolejno przeglada
       katalogi podane przez uzytkownika i sprawdza, czy wartosc 
       identyfikujaca zasob nie jest dwuznaczna - w takim przypadku powiadamia
       wywolujacego metode obiektem wyjatku typu ResourceFileIdAmbiguous

     @exception
       ResourceFileIdAmbiguous
   */
   Handle getResource(const std::string& fileName);
   
   /**
     @brief
       Wyszukanie, oraz ewentualne pozyskanie zasobu okreslonego
       identyfikatorem definiujacym dokladne miejsce poszukiwan
       w hierarchii katalogow

     @see
       Handle getResource(const std::string& fileName)
   */
   Handle getResource(const std::string& fileName,
                      const std::string& directory);
    
   /**
     @brief
       Przegladniecie zadanego katalogu w poszukiwaniu plikow zasobow 
       pasujacych do kryterium okreslonego w konstruktorze - zadanego
       rozszerzenia plikow zasobow. Proces ten wykonywany jest iteracyjnie,
       a kazda kolejna proba daje odpowiedz, ktorego zrodlem jest zapytanie
       bazujace na sprawdzeniu poprawnosci dopasowania rozszezenia nazwy pliku.
       Pozytywny rezultat owocuje utworzeniem obiektu posrednika, ktory
       jest spoiwem w relacji klient-zasob. Komunikat wyslany przez uzytkownika
       w celu uzyskania pozadanych efektow moze implikowac potrzebe 
       pozyskania zasobu w celu udostepnienia danych.
   */
   void loadResourcesFromDir(const std::string& path);

   /**
     @brief
       Wykonanie wczytania zasobow znajdujacych sie w poddrzewie katalogow
       o korzeniu oznaczonym przez zmienna rootPathNode
   */
   void loadResourcesFromTree(const std::string& rootPathNode);
   
   /**
     @brief
       Poszukiwanie ewentualnych plikow zasobow w katalogach wyszczegolnionych
       przez klienta (uzytkownika menadzera) za pomoca funkcji nazwanych 
       odpowiednio: setResourcesSearchDir, oraz removeResourcesSearchDir
   */
   void loadResources();

   /**
     @brief
   */
   void acquireResourcesFromDir(const std::string& path);

   /**
     @brief
   */
   void acquireResourcesFromTree(const std::string& path);

   /**
     @brief
   */
   void acquireResources();

   /**
     @brief
       Zwolnienie pamieci przydzielonej zasobom znajdujacym
       sie w zadanym katalogu bedacym argumentem wywolania
   */
   void releaseResourcesFromDir(const std::string& path);

   /**
     @brief
   */
   void releaseResourcesFromTree(const std::string& path);

   /**
     @brief
       Zwolnienie pamieci przydzielonej wszystkim aktualnie 
       pobranym zasobom menadzera
   */
   void releaseResources();

   /**
     @brief
       Zwolnienie calej pamieci menadzera odnoszacej sie do zasobow,
       reprezentujacych nich posrednikow, zbioru katalogow - jednym slowem
       doprowadzenie menadzera do pierwotnego, surowego stanu. Po wywolaniu
       metody wszystkie uchwyty w jednej chwili staja sie niewazne. Proba
       odwolania sie za posrednictwem uniewaznionego uchwytu doprowadzi
       do powstania bledu czasu wykonywania
   */
   void purgeAll();

 private:
   void insertResource(const SharedResource& resource);
   void removeResource(const SharedResource& resource);

 private:
   // Trzy glowne typy zewnetrznie utworzonych instancji. Hermetyzacja ich
   // zachowan sprzyja latwej wymianie podczas przenoszenia kodu na inne
   // platformy, oraz podczas jego rozwijania - za sprawa instancji typu
   // ResourceManage. Kolejno, instancja pierwszego typu odpowiada za
   // wyliczenie elementow wchodzacych w sklad struktury katalogow/plikow
   // o korzeniu zadanym przez klienta. Drugi obiekt obsluguje zadania 
   // dotyczace aktualnego czasu systemowego, ktorego wartosc sluzy jako
   // kryterium podczas organizowania obiektow posredniczacych w dostepie
   // do zasobow. Trzeci zewnetrzny obiekt jest zdefiniowany na potrzeby
   // przyszlych implementacji, korzystajacych z zarzadcy. Dzieki 
   // przedefiniowaniu bazowego interfejsu dostajemy mozliwosc wprowadzania
   // wlasnych rozszezen, podczas operacji takich jak: pozyskiwanie, ladowanie,
   // zwalnianie, usuwanie zasobu. Udostepniona funkcjonalnosc tego rodzaju 
   // moze okazac sie pomocna podczas przeprowadzania pierwotnej inicjalizacji
   // zasobow. Dla zobrazowania mozna wykorzystac sytuacje wczytywania danych 
   // danego poziomu gry. Informacje na temat aktualnie wczytywanych zasobow
   // wyswietlane sa poprzez obiekt realizujacy interfejs.

   typedef std::auto_ptr<DriveElementsEnumerator> DriveElementsEnumeratorAutoPtr;
   typedef std::auto_ptr<TimeInformation>         TimeInformationAutoPtr;
   typedef std::auto_ptr<ResourceManage>          ResourceManageAutoPtr;

   DriveElementsEnumeratorAutoPtr  elementsEnum_;
   TimeInformationAutoPtr          timeInformation_;
   ResourceManageAutoPtr           resourceManage_;
   
 private:
   // Kolejne definicje synonimow zostaly pogrupowane wedlug kryterium 
   // realacji miedzy nimi zawartych. Obiekty pierwszego zbioru wchodza
   // w sklad zakresu okreslajacego dostepne katalogi, ktore brane sa podczas 
   // rozrozstrzygania dopasowania zasobu do zadanego identyfikatora, oraz
   // w czasie ladowania wszystkich zasobow (sa to funkcje getResource, oraz
   // loadResources).
   // Obiekt nastepnego w kolejnosci typu - ResourcesSet - jest odpowiedzialny
   // za przechowywanie obiektow posrednikow. Kryterium ich sortowania jest
   // zdeterminowane poprzez metode operator< znajdujaca sie w zakresie 
   // klasy SharedResource. Kontener asocjacyjny zapewnia prawidlowa organizacje
   // elementow, ktora jest podstawa dzialania menadzera, przejawiajaca sie
   // uszeregowaniem zasobow od najzadziej do najczesciej uzywanego, lacznie
   // z uwzglednieniem wartosci priorytetu
   // W sklad trzeciego zestawu wchodza typy, ktore determinuja zrodlo zasobu
   // w postaci sciezki dostepu (para katalog, nazwa pliku)
   // Typ TSTHandleToPath, oraz skojarzony z nim iterator odwzorowuja
   // zadany uchwyt (instancja typu zdefiniowanego w klasie struktury drzewa 
   // TST) na sciezke katalogow zasobu, ktory jest identyfikowany wartoscia 
   // uchwytu
   // Ostatnie odwzorowanie umozliwia pobranie adresu instancji posrednika
   // bedac w posiadaniu uchwytu TST sprzezonego z danym zasobem

   typedef std::string              DirectoryName;
   typedef std::set<DirectoryName>  Directories;
   typedef Directories::iterator    DirectoriesItor;
   
   typedef std::set<SharedResource>         ResourcesSet;
   typedef typename ResourcesSet::iterator  ResourcesSetItor;

   typedef std::string                      FileName;
   typedef std::string                      Directory;
   typedef std::pair<Directory, FileName>   Path;

   typedef std::map<TSTHandle, Path>             TSTHandleToPath;
   typedef typename TSTHandleToPath::iterator    TSTHandleToPathItor;
   typedef typename TSTHandleToPath::value_type  TSTHandleToPathValueType;
   
   typedef std::map<TSTHandle, const SharedResource*>  TSTHandleToShrRes;
   typedef typename TSTHandleToShrRes::iterator        TSTHandleToShrResItor;

   typedef std::auto_ptr<ElementMatcher>  ElementMatcherAutoPtr;
   
   size_t  maxMemory_;     /**< Maksymalny dostepny rozmiar pamieci */
   size_t  usedMemory_;    /**< Aktualnie wykorzystywany obszar pamieci */
   
   Directories   searchDir_;  /**< Katalogi wykorzystywane podczas dopasowania */
   ResourcesSet  resources_;  /**< Zestaw posrednikow zasobow */
   
   PathToTSTHandle    pathToTSTHandle_;   /**< Sciezka zasobu na uchwyt TST */
   TSTHandleToShrRes  tstHandleToShrRes_; /**< Uchwyt TST na posrednika zasobu */
   TSTHandleToPath    tstHandleToPath_;   /**< Odwzorowanie uchwytu struktury 
                                               trynarnej na sciezke zasobu */

   ElementMatcherAutoPtr  elementMatcher_; /**< Kryterium wykorzystywane podczas
                                                proby pobrani zasobu */

 private:
   bool isDirectoryValid( const DriveElementsEnumerator::ItorAutoPtr& elemItor );

 private:
   // Nie zdefiniowane
   DataFileResourceManager(const DataFileResourceManager&);
   DataFileResourceManager& operator =(const DataFileResourceManager&);   
};

//
template<typename RESOURCE, typename FILE_FORMAT>
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::SharedResource(DataFileResourceManager&  owner,
                 const std::string&        dataFileName,
                 const std::string&        dataFilePath)
  : owner_(owner)
  , handle_(owner.pathToTSTHandle_.getHandle(dataFilePath + "\\" + dataFileName))
  , priority_(MEDIUM_PRIORITY)
  , referenceCounter_(1)
  , resource_(0)
  , lastAccess_(owner.timeInformation_->getSystemTime())
{
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::acquire() const
{
  // Aby pozyskac zasob, trzeba sprawdzic czy aktualnie nie znajduje sie 
  // on w pamieci obslugiwanej przez zarzadce. Jesli menadzer zwolil obszar 
  // danych mu przynaleznych na rzecz innego pozyskiwanego zasobu, 
  // wowczas nalezy ponownie wczytac go z zewnetrznego zrodla. Aby prawidlowo
  // wykonac to zadanie trzeba zdeszyfrowac wartosc uchwytu okreslajaca
  // jednoznacznie sciezke dostepu do zewnetrznych danych zasobu. Po wykonaniu
  // tej czynnosci, wystarczy przekazac odpowiedzialnosc nowo utworzonemu 
  // wspoldzielonemu obiektowi, polegajaca na wczytaniu swoich danych
  
  if( !isLoaded() )
  {
    Path& pathPair = owner_.tstHandleToPath_[ handle_ ];
    const Directory& DIRECTORY = pathPair.first;
    const FileName&  FILE_NAME = pathPair.second;
    std::string      PATH = DIRECTORY + "\\" + FILE_NAME;
    std::ifstream input(PATH.c_str(), std::ios_base::binary);
    if(!input)
      throw FailOnResourceAcquireException(PATH);
    resource_ = ResourceAutoPtr( new RESOURCE(input) );
  }

  Utilities::assertion( isLoaded(),
    "Obiekt nie zostal wczytany! Blad wewnetrzny!" );
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::release() const
{
  // Operacja zwolnienia pamieci przydzielonej zasobowi polega na 
  // oproznieniu obszaru pamieci udostepnionej zbiorowi danych 
  // tworzacych wspoldzielona jego czesc

  if( isLoaded() ) 
    resource_ = ResourceAutoPtr(0);

  Utilities::assertion( resource_.get() == 0, 
    "Zwolnienie zasobu zakonczone niepowodzeniem!" );
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::increaseRef() const
{ 
  Utilities::assertion(
    referenceCounter_ != std::numeric_limits<RefCounter>::max(),
    "Maksymalna wartosc licznika odwolan! Mozliwy blad przepelnienia!" );

  ++referenceCounter_; 
}
  
//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::decreaseRef() const
{
  // Wspoldzielony zasob zawsze posiada przynajmniej wartosc rowna
  // jednosci wynikla z posiadania przez menadzer prawa wlasnosci do zasobu

//  Utilities::assertion( referenceCounter_ <= 1, 
//  "Licznik odwolan zasobu posiada bledna wartosc!" );

  --referenceCounter_; 
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::setPriority(ResourcePriority priority) const
{
  // Jesli biezacy priorytet posiada rozna wartosc od pozadanej przez
  // klienta, ktory inicjuje komunikat, wowczas nalezy przystapic do jego
  // zmiany. Implikacja calego zajscia jest zmiana pozycji zasobu 
  // w posiadajacym do niego prawa wlasnosci agregacie-menadzerze,
  // ktora wykonywana jest za posrednictwem wywolania metody update()

  if(priority_ != priority) {
    priority_ = priority; 
    update();
  }
}

//
template<typename RESOURCE, typename FILE_FORMAT>
typename DataFileResourceManager<RESOURCE, FILE_FORMAT>::ResourcePriority 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::getPriority() const 
{ 
  return priority_; 
}
  
//
template<typename RESOURCE, typename FILE_FORMAT>
size_t
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::getByteSize() const 
{
  // Do prawidlowego dzialania funkcji jest wymagane zdefiniowanie
  // przez uzytkownika metody getByteSize() w klasie zasobu

  update();
  return resource_->getByteSize();
}

//
template<typename RESOURCE, typename FILE_FORMAT>
bool 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::isShared() const 
{ 
  return referenceCounter_ > 1; 
}

//
template<typename RESOURCE, typename FILE_FORMAT>
bool 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::operator <(const SharedResource& rhs) const 
{
  if( priority_ < rhs.priority_ ) 
    return true;
  else if( priority_ > rhs.priority_ )
    return false;
  else { // zasoby o rownych priorytetach
    if( lastAccess_ < rhs.lastAccess_ )
      return true;
    else if( lastAccess_ > rhs.lastAccess_ )
      return false;
    else // rowne czasy ostatniego dostepu (malo prawdopodobne)
      return false;
  }
}

//
template<typename RESOURCE, typename FILE_FORMAT>
const RESOURCE& 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::getResource() const
{
  update();
  return *resource_;
}

//
template<typename RESOURCE, typename FILE_FORMAT>
typename DataFileResourceManager<RESOURCE, FILE_FORMAT>::TSTHandle 
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::getTstHandle() const
{
  return handle_;
}

//
template<typename RESOURCE, typename FILE_FORMAT>
bool
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::isLoaded() const 
{ 
  return resource_.get() != 0; 
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void
DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource
::update() const
{
  // Przy kazdym odwolaniu do zasobu jest uwzgledniany aktualny
  // czas, ktorego wartosc jest jednym z czynnikow kryterium
  // sortowania w menadzerze. Dzieki aktualizacji tej wartosci
  // w sytuacji niedoboru pamieci podczas pobierania nowych zasobow,
  // obiekty bedace nieuzywane od dluzszego czasu sa zwalniane,
  // tym samym umozliwiajac pobranie pozadanych zewnetrznych danych
  // zasobow.
  // Proces wykonania aktualizacji struktury odwzorowujacej
  // uszeregowana sekwencje, w ktorej nieuzytki, oraz obiekty
  // najczesciej wykorzystywane sa wzgledem siebie posortowane jest
  // podyktowany wewnetrzna reprezentacja struktury zbioru. Aby
  // etap sortowania przebiegl w prawidlowy sposob, z agregatu
  // najpierw usuwany jest obiekt udostepnianego zasobu, nastepnie
  // po wykonaniu aktualizacji czasu jest on spowrotem umieszczany
  // w kolekcji na nowej - okreslonej przez kryterium - pozycji.

  if( !isLoaded() )
    acquire();

  // Kopiowanie obiektu wspoldzielonego zasobu w tym wypadku nie jest
  // pracochlonnym zadaniem, poniewaz wiekszosc z danych skladowych
  // jest typu wbudowanego. Kopiowanie inteligentnego wskaznika
  // reprezentujacego obiekt zasobu rowniez jest bezpieczne, poniewaz
  // prawa wlasnosci sa poprawnie przekazywane z, oraz do obiektow
  // bioracych udzial w operacji

  SharedResource resourceCopy(*this);
  resourceCopy.lastAccess_ = owner_.timeInformation_->getSystemTime();
  owner_.insertResource(resourceCopy);
  owner_.removeResource(*this);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
DataFileResourceManager<RESOURCE, FILE_FORMAT>::Handle
::Handle(DataFileResourceManager&  owner,
         TSTHandle                 handle)
  : owner_(owner)
  , handle_(handle)
{
  dereferenceResource().increaseRef();
}

//
template<typename RESOURCE, typename FILE_FORMAT>
typename const DataFileResourceManager<RESOURCE, FILE_FORMAT>::SharedResource&
DataFileResourceManager<RESOURCE, FILE_FORMAT>::Handle
::dereferenceResource() const
{
  return *( owner_.tstHandleToShrRes_[ handle_ ] );
}

//
template<typename RESOURCE, typename FILE_FORMAT>
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::DataFileResourceManager(DriveElementsEnumerator*  fileEnumerator,
                          TimeInformation*          timeInformation)
  : elementsEnum_(fileEnumerator)
  , timeInformation_(timeInformation)
  , resourceManage_(createNullResourceManage())
  , elementMatcher_(createNullElementMatcher())
  , usedMemory_(0)
{
  const int ONE_MEGABYTE = 1024;
  maxMemory_ = ONE_MEGABYTE;
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::setResourceManage(ResourceManage* resourceManage)
{
  resourceManage_ = ResourceManageAutoPtr(resourceManage);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::setElementMatcher(ElementMatcher* elementMatcher)
{
  elementMatcher_ = ElementMatcherAutoPtr(elementMatcher);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::setMemoryMaximum(size_t memorySize)
{
  maxMemory_ = memorySize;
}

//
template<typename RESOURCE, typename FILE_FORMAT>
size_t 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::getMemoryMaximum() const
{
  return maxMemory_;
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::setResourcesSearchDir(const std::string& path)
{
  searchDir_.insert(path);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
bool 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::removeResourcesSearchDir(const std::string& path)
{
  return searchDir_.erase(path) != 0;
}

template<typename RESOURCE, typename FILE_FORMAT>
bool 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::isDirectoryValid( const DriveElementsEnumerator::ItorAutoPtr& elemItor )
{
  return elemItor->isDirectory() &&
    elemItor->getElementName() != "." &&
    elemItor->getElementName() != "..";
}

//
template<typename RESOURCE, typename FILE_FORMAT>
typename DataFileResourceManager<RESOURCE, FILE_FORMAT>::Handle 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::getResource(const std::string& fileName)
{
  // Z pobraniem uchwytu do zasobu jedynie na podstawie nazwy pliku
  // wiaze sie mozliwosc powstania dwuznacznego dopasowania identyfikatora
  // (nazwy pliku) do danych zasobu.
  // Aby wykonac dopasowanie nalezy wykonac trawersacje przez wszystkie
  // zarejestrowane w obiekcie menadzera katalogi. W kazdej kolejnej iteracji
  // poszukiwany jest odpowiadajacy identyfikatorowi zasob. Jesli zadana 
  // nazwa zasobu bedzie odwzorowana wiecej niz raz, zdarzenie to zostanie 
  // zgloszone w postaci obiektu wyjatku informujacego o niemoznosci 
  // jednoznacznego dopasowania identyfikatora do zasobu

  bool handleAlreadyExists = false;
  TSTHandle tstHandle;

  for(DirectoriesItor dirItor = searchDir_.begin();
    dirItor != searchDir_.end();
    ++dirItor)
  {
    std::string path = *dirItor + "\\" + fileName;
    
    if( pathToTSTHandle_.isIdMapped(path) )
    {
      if( handleAlreadyExists )
        throw createResourceFileIdAmbiguousException(fileName);
      
      handleAlreadyExists = true;
      
      tstHandle = pathToTSTHandle_.getHandle(path);
    }
  }
  
  if( handleAlreadyExists )
    return Handle(*this, tstHandle);
  else
    throw createUnvalidIdException(fileName);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
typename DataFileResourceManager<RESOURCE, FILE_FORMAT>::Handle 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::getResource(const std::string& fileName,
              const std::string& directory)
{
  // Chcac pobrac uchwyt do zasobu o zadanym identyfikatorze, oraz
  // sciezce okreslajacej katalog, w ktorym sie znajduje nalezy
  // sprawdzic, czy wartosc konkatencji argumentow wywolania funkcji
  // (pelna sciezka okreslajace zrodlo danych zasobu) jest
  // odwzorowana w strukturze drzewa TST danych menadzera.
  // Odwzorowanie zwroci wartosc prawda wtedy i tylko wtedy, gdy
  // zasob byl w przeszlosci wczytywany. Jest to rownoznaczne
  // ze stwierdzeniem, iz istnieje mozliwosc pobrania jego uchwytu,
  // dzieki ktoremu wywolujacy metode otrzymuje mozliwosc pobierania
  // danych zasobu

  const std::string PATH = directory + "\\" + fileName;

  if( pathToTSTHandle_.isIdMapped(PATH) )
    return Handle( *this, pathToTSTHandle_.getHandle(PATH) );
  else
    throw createUnvalidIdException(fileName);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::purgeAll()
{
  const size_t  EMPTY = 0;
  usedMemory_ = EMPTY;
   
  Directories().swap(searchDir_);
  ResourcesSet().swap(resources_);
  TSTHandleToPath().swap(tstHandleToPath_);
  TSTHandleToShrRes().swap(tstHandleToShrRes_);
  PathToTSTHandle().swap(pathToTSTHandle_);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::loadResourcesFromDir(const std::string& path)
{
  DriveElementsEnumeratorAutoPtr dirElemsEnum( elementsEnum_->clone() );
  dirElemsEnum->setDirectory(path);

  for(DriveElementsEnumerator::ItorAutoPtr fileEnum(dirElemsEnum->getItor());
    fileEnum->isValid();
    fileEnum->moveToNextElement())
  {
    const std::string& FILE_OR_DIR_ELEMENT = fileEnum->getElementName();
    if( elementMatcher_->match(FILE_OR_DIR_ELEMENT) &&
      !pathToTSTHandle_.isIdMapped(path + "\\" + FILE_OR_DIR_ELEMENT) ) 
    {
      resourceManage_->load(path, FILE_OR_DIR_ELEMENT);
      SharedResource res(*this, FILE_OR_DIR_ELEMENT, path);
      insertResource( res );
      tstHandleToPath_.insert( TSTHandleToPathValueType( 
        res.getTstHandle(),
        std::make_pair(path, FILE_OR_DIR_ELEMENT) )
      );
    }
  }
  setResourcesSearchDir(path);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::loadResourcesFromTree(const std::string& rootPathNode)
{
  // Metoda wykonuje funkcje wczytania zasobow (spelniajacych okreslone
  // kryterium) znajdujacych sie w strukturze katalogow, ktorej korzeniem
  // jest zadana przez argument wywolania wartosc determinujaca poczatkowa
  // sciezke (katalog) poszukiwan.

  const std::string& PATH = rootPathNode;

  DriveElementsEnumeratorAutoPtr dirElemsEnum( elementsEnum_->clone() );
  dirElemsEnum->setDirectory(PATH);
  
  // Aby wykonac ewentualne wczytanie zasobow w pierwszej kolejnosci
  // nalezy sprawdzic biezacy katalog w poszukiwaniu elementow 
  // spelniajacych okreslone kryterium (np. dopasowanie nazwy pliku 
  // do zadanego rozszezenia).
  
  // Oczywiscie jezeli aktualnie sprawdzany element jest katalogiem
  // wowczas wchodzimy w rekurencyjne wywolania, aby przejzec wszystkie
  // odnogi katalogow.
  
  // Gdy trawersowany element spelnia warunek okreslony przez instancje 
  // typu ElementMatcher, mozemy dokonac wczytania zasobu. W pierwszej
  // kolejnosci dajemy mozliwosc uzytkownikowi zareagowania na 
  // przeprowadzane dzialanie. Nastepnie tworzymy posrednika, ktory 
  // reprezentuje zasob, oraz powiadamiamy wewnetrzne struktury
  // menadzera o zaistnialym zdarzeniu

  for(DriveElementsEnumerator::ItorAutoPtr elemItor( dirElemsEnum->getItor() );
    elemItor->isValid();
    elemItor->moveToNextElement())
  {
    if( isDirectoryValid(elemItor) ) 
    {
      loadResourcesFromTree( PATH + "\\" + elemItor->getElementName() );
    }
    else 
    {
      const std::string& FILE_ELEMENT = elemItor->getElementName();
  
      if( elementMatcher_->match(FILE_ELEMENT) &&
        !pathToTSTHandle_.isIdMapped(PATH + "\\" + FILE_ELEMENT) ) 
      {
        resourceManage_->load(PATH, FILE_ELEMENT);

        // Tworzac obiekt posrednika wykonuje on prace informujaca
        // wewnetrzna strukture TST menadzera o nowej sciezce zasobu.
        // Ustalony zostaje priorytet zasobu rowny sredniej wartosci.
        // Wartosc udostepnionego czasu systemowego sluzy do inicjalizacji
        // zmiennej okreslajacej ostatni czas dostepu do zasobu

        SharedResource proxyResource(*this, FILE_ELEMENT, PATH);

        insertResource( proxyResource );

        tstHandleToPath_.insert( 
          TSTHandleToPathValueType( 
            proxyResource.getTstHandle(),
            std::make_pair(PATH, FILE_ELEMENT) 
          )
        );
      
        setResourcesSearchDir(PATH);
      }
    }
  }
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::loadResources()
{
  // Aby odczytac wszystkie zasoby znajdujace sie w wyszczegolnionych
  // katalogach (wykorzystanie metody setResourcesSearchDir) nalezy
  // dla kazdego z nich wykonac funkcje odczytu zasobow z zadanego
  // katalogu. Zasoby, ktore beda wczytane musza spelniac zadane kryterium
  // okreslone przez obiekt typu ElementMatcher

  for(DirectoriesItor dirItor = searchDir_.begin();
    dirItor != searchDir_.end();
    ++dirItor)
  {
    const std::string& DIRECTORY_PATH = *dirItor;
    loadResourcesFromDir( DIRECTORY_PATH );
  }
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::acquireResourcesFromDir(const std::string& path)
{
  elementsEnum_->setDirectory(path);

  for(DriveElementsEnumerator::ItorAutoPtr fileEnum(elementsEnum_->getItor());
    fileEnum->isValid();
    fileEnum->moveToNextElement())
  {
    SharedResource resource(*this, fileEnum->getElementName(), path);
    resource.acquire();
    insertResource( resource );
    tstHandleToPath_.insert( TSTHandleToPathValueType( 
      resource.getTstHandle(),
      std::make_pair(path, fileEnum->getElementName()) )
    );
  }

  setResourcesSearchDir(path);
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::insertResource(const SharedResource& resource)
{  
  ResourcesSetItor newResourcePos = resources_.insert(resource).first;
  tstHandleToShrRes_.insert(
    TSTHandleToShrRes::value_type(resource.getTstHandle(), &*newResourcePos) );
}

//
template<typename RESOURCE, typename FILE_FORMAT>
void 
DataFileResourceManager<RESOURCE, FILE_FORMAT>
::removeResource(const SharedResource& resource)
{
  tstHandleToShrRes_.erase(resource.getTstHandle());
  resources_.erase(resource);
}

} // namespace Platform
} // namespace Gcad

#endif

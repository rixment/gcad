#include "GcadDriveElementsEnumeratorWin32.h"
#include "GcadAssertion.h"

namespace Gcad {
namespace Platform {

  // IMPLEMENTACJA ITERATORA GLOWNEJ STRUKTURY

DriveElementsEnumeratorWin32::Itor 
::Itor(const DriveElementsEnumeratorWin32& owner)
  : owner_(owner)
  , currDir_(0)
{
  moveToBegin();
}

DriveElementsEnumeratorWin32::Itor
::~Itor()
{
  // Podstawowym zadaniem destruktora jest zwolnienie zasobow pozyskanych
  // za posrednictwem wywolan funkcji Win32API. Udostepnione zasoby sa 
  // zarzadzane za pomoca uchwytu zwroconego przez odpowiednie sluzby Win32API

  FindClose(currDir_);
}

bool
DriveElementsEnumeratorWin32::Itor 
::isDirectory() const
{
  // Sprawdzenie polegajace na ustaleniu tozsamosci elementu jest wykonywane
  // poprzez bitowa koniunkcje determinujaca wlasciwosc - w tym wypadku jest
  // to sprawdzenie polegajace na ustaleniu informacji bedacej odpowiedziom
  // na pytanie: czy dany element znajdujacy sie w strukturze jest katalogiem?

  return (data_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 
    static_cast<DWORD>(0);
}

std::string
DriveElementsEnumeratorWin32::Itor 
::getElementName() const
{
  return data_.cFileName;
}

bool
DriveElementsEnumeratorWin32::Itor 
::moveToNextElement()
{
  // Bezspornym wymaganiem, ktore musi zostac spelnione jest istnienie uchwytu
  // udostepnionego przez system Win32API. Przejscie do nastepnego elementu
  // struktury plikow/katalogow jest mozliwe dzieki funkcji Win32API 
  // FindNextFile, pobierajacej uchwyt, oraz skojarzona z nia strukture.
  // Jesli operacja zostanie uwienczona powodzeniem, wowczas funkcja zwraca
  // wartosc rozna od zera. W przeciwnym wypadku (rezultat rowny zeru) informacja
  // zwrotna opisuje zaistnienie sytuacji zakonczenia sprawdzania struktury
  // katalogow. W zwiazku z tym faktem, nastepuje zwolnienie przydzielonego
  // zasobu, oraz zwrocenie informacji na zewnatrz funkcji oznajmiajacej
  // zakonczenie iteracyjnego procesu

  Utilities::assertion(currDir_ != 0, "Iterator nie ustawiony!");

  if( FindNextFile(currDir_, &data_) != 0 )
    return true;
  else {
    FindClose(currDir_);
    currDir_ = 0;
    return false;
  }
}

bool
DriveElementsEnumeratorWin32::Itor 
::isValid() const
{
  return currDir_ != 0;
}

void
DriveElementsEnumeratorWin32::Itor 
::moveToBegin()
{
  // Aby ustawic wskaznik elementow struktury na poczatkowym elemencie
  // wykonywana jest w pierwszej kolejnosci operacja zwolnienia zasobow
  // skojarzonych z aktualnym uchwytem (jesli oczywiscie takowe sa pozyskane).
  // W nastepnej kolejnosci wykonujemy szereg zapytan (odwolujemy sie do 
  // skladowych struktury glownego obiektu) dajacych w rezultacie 
  // wzgledna sciezke przeszukiwan elementow. Ostatnim krokiem wienczacym
  // dzialanie funkcji jest wywolanie funkcji Win32API zwracajacej poprawny
  // uchwyt do zasobu okreslajacego zadany korzen struktury plikow/katalogow

  if(currDir_ != 0)
    FindClose(currDir_);

  const std::string  PATH( owner_.baseDir_ + "\\*.*" );
  
  currDir_ = FindFirstFile( PATH.c_str(), &data_ );
}

  // IMPLEMENTACJA GLOWNEGO OBIEKTU ZARZADZAJACEGO
    
DriveElementsEnumeratorWin32
::DriveElementsEnumeratorWin32(const std::string& directory) 
  : baseDir_(directory)
{
}

DriveElementsEnumeratorWin32* 
DriveElementsEnumeratorWin32
::clone() const
{
  return new DriveElementsEnumeratorWin32(baseDir_);
}

DriveElementsEnumeratorWin32::ItorAutoPtr 
DriveElementsEnumeratorWin32
::getItor() const
{
  return ItorAutoPtr( new Itor(*this) );
}

void 
DriveElementsEnumeratorWin32
::setDirectory(const std::string& newDirectory) 
{
  baseDir_ = newDirectory;
}
   
} // namespace Platform
} // namespace Gcad
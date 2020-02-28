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

#ifndef _GCAD_PLANEUTIL_H_
#define _GCAD_PLANEUTIL_H_

#include "GcadAssertion.h"
#include "GcadParamLine3.h"
#include "GcadParamLine3Util.h"
#include "GcadPlane.h"
#include "GcadVector3.h"
#include "GcadVector3Util.h"

#include <cmath>

namespace Gcad {
namespace Math {

/** 
  @brief
    Zestaw funkcji operujacych na plaszczyznach
*/
struct PlaneUtil {

  /** 
    @brief Typ determinujacy polozenie punktu wzgledem plaszczyzny
  */
  enum EPointSide {       
    EPS_ON,    /**< Punkt nalezy do plaszczyzny */
    EPS_FRONT, /**< Punkt lezy w dodatniej polprzestrzeni */           
    EPS_BACK   /**< Punkt lezy w ujemnej polprzestrzeni */
  };

  /** 
    @brief Typ determinujacy wynik przeciecia plaszczyzny z prosta
  */
  enum EIntersectResult { 
    EIR_ON,       /**< Prosta znajduje sie na plaszczyznie */
    EIR_PARALLEL, /**< Prosta jest rownolegla wzgledem plaszczyzny */
    EIR_IN,       /**< Odcinek przecina plaszczyzne */
    EIR_OUT       /**< Prosta przecina plaszczyzne */
  };

  /** 
    @brief Okreslenie polozenia punktu wzgledem plaszczyzny
  */
  static EPointSide
    pointSide( const Plane&    plane, 
               const Vector3&  point );

  /** 
    @brief 
      Obliczenie wartosci okreslajacej najblizsza odleglosc
      zawarta pomiedzy punktem w przestrzeni, a plaszczyzna
  */
  static REAL
    nearestDistance( const Plane&    plane, 
                     const Vector3&  point );

  /** 
    @brief Obliczenie wartosci rzutu punktu na plaszczyzne 
  */
  static void
    nearestPoint( const Plane&    plane, 
                  const Vector3&  point, 
                  Vector3*        outPoint );

  /**
    @brief Okreslenie, czy dana parametryczna prosta przecina plaszczyzne
  */
  static bool 
    isIntersect( const Plane&       plane, 
                 const ParamLine3&  line );

  /** 
    @brief 
      Oblicza punkt przeciecia prostej z plaszczyzna.
    
    @return 
      Zwrocona wartosc determinuje pewna abstrakcje, okreslajaca
      hipotetyczne miejsce przeciecia, jesli tak owe nastapilo

    @remark 
      Oblicza punkt przeciecia prostej z plaszczyzna 
      (o ile tak owy istnieje), poprzez ustawienie czynnika t 
      parametrycznej prostej, przekazanej jako argument wywolania
  */
  static EIntersectResult 
    intersect( const Plane&  plane,
               ParamLine3*   line );

  /** 
    @brief 
      Oblicza punkt przeciecia odcinka wyznaczonego przez 
      dwa punkty z plaszczyzna. 
    
    @return 
      W wypadku nastapienia przeciecia prostej z plaszczyzna
      wartosc zwrocona przez funkcje jest rowna prawdzie. W przeciwnym
      przypadku okreslona jest falszem (prosta widocznie jest rownolegla
      wzgledem plaszczyzny      

    @remark 
      Jedynie w wypadku nastapienia przeciecia, zmienna <i>point</i>
      zostanie okreslona - bedzie determinowac punkt przeciecia
  */
  static bool
    computePoint( const Plane&       plane, 
                  const ParamLine3&  line, 
                  Vector3*           point );

  /** 
    @brief 
      Oblicza punkt odbicia przy zalozeniu, iz prosta 
      przecina plaszczyzne

    @return 
      Polozenie refleksu w przestrzeni, okreslonego
      poprzez parametryczna prosta (odcinek) przecinajaca
      plaszczyzne
  */
  static Vector3 
    computeReflectPoint( const Plane&       plane,
                         const ParamLine3&  line );

 private:
   // nie zaimplementowane
   PlaneUtil();
   PlaneUtil( const PlaneUtil& );
   PlaneUtil& operator =( const PlaneUtil& );

};


//  Zwracana przez funkcje wartosc okresla polozenie (wzgledem plaszczyzny)
//  punktu przekazanego, jako argument wywolania. Dla strony zawartego w czesci:
//    - dodatniej (przedniej) zwraca wartosc - FrontPlane. ( n . v ) > 0
//    - ujemnej (tylniej) zwraca wartosc - BackPlane. ( n . v ) < 0
//    - na plaszczyznie zwraca wartosc - OnPlane. ( n . v ) == 0
//  Twierdzenia powyzszych wzorow:
//    Jak wiadomo wartosc iloczynu skalarnego znormalizowanych dwoch wektorow
//  (w tym wypadku wektorow, z ktorych jeden jest wektorem znormalizowanym
//  okreslajacym kierunek plaszczyzny, natomias drugi jest wynikiem odjecia
//  sprawdzanego punktu, od wartosci punktu zawartego na plaszyznie)
//  mozna utozsamic z wartoscia funkcji cos dla kata zawartego miedzy nimi.
//  Implikacja powyzszego twierdzenia sa nastepujace wnioski:
//    - gdy dwa wektory tworza kat prosty, wartosc cos wynosi 0. Tym samym
//      opisane wyzej dwa wektory tworza kat prosty, jesli poddawany
//      sprawdzeniu punkt lezy na plaszczyznie
//    - gdy dwa wektory tworza kat ostry, wartosc cos jest > 0, czyli
//      analizowany punkt lezy po stronie dodatniej (przedniej) plaszczyzny
//    - twierdzenie odwrotnie proporcjonalnie do powyzszego dla cos < 0
//    Aby nie tracic kilku cykli zegarowych nie jest przeprowadzana normalizacja
//  wyliczonego wektora (zaczepionego od punktu plaszczyzny do sprawdzanego
//  punktu). Uzyskany wynik rownie dobrze moze posluzyc do okreslenia strony
//  plaszczyzny, po ktorej znajduje sie analizowany punkt, tyle ze nie bedzie
//  go mozna utozsamic z wartoscia funkcji cosinus.
//    Aby to zrobic mozna wartosc iloczynu skalarnego pomnozyc przez dlugosc
//  wektora, ktory nie zostal znormalizowany. Wowczas wartosc cosinusa kata
//  bedzie rowna iloczynowi
//
//  n . v = | n | * | v | * cos( theta )
//
//  | n | == 1 - wektor jednostkowy
//  | v | != 1 - brak normalizacji
//
//  n . v = | v | * cos ( theta )
//
//  gdy | v | == 1, n . v = cos( theta )
//-------------------------------------------------------------------------------

PlaneUtil::EPointSide PlaneUtil
::pointSide( const Plane&    plane, 
             const Vector3&  point ) 
{
  // oblicz wektor zbudowany na bazie punktu nalezacego do plaszczyzny
  // oraz punkty poddawanego analizie polozenia wzgledem plaszczyzny
  Vector3  computedVec( point - plane.p0() );

  // oblicz dla wyliczonego wektora i wektora plaszczyzny, iloczyn skalarny
  Real sideValue = Vector3Util::dot( plane.n(), computedVec );

  // na podstawie wartosci iloczynu ustal polozenie punktu wzgledem plaszczyzny
  if( sideValue < 1e-5 && sideValue > -1e-5 )
    return EPS_ON;
  else return sideValue > 0 ? EPS_FRONT : EPS_BACK;
}
            
// Oblicza minimalna odleglosc, tzw. rzut wektora, zawarta pomiedzy
// plaszczyzna, a punktem.
// distance = ( point - plane_point ) . plane_normal
// source: "Perelki programowania gier" - tom II str. 220
//-------------------------------------------------------------------------------

Real PlaneUtil
::nearestDistance( const Plane&    plane,
                   const Vector3&  point ) 
{
  // oblicz wektor skierowany od punktu lezacego na powierzchni do
  // wektora okreslajacego punkt, wzgledem ktorego bedzie obliczana
  // odleglosc
  Vector3  computedVector( point - plane.p0() );

  // oblicz iloczyn skalarny wektora normalnego powierzchni (wektor musi
  // byc znormalizowany) i wyliczonego powyzej wektora
  Real length = Vector3Util::dot( plane.n(), computedVector );

  // przeprowadz operacje wyliczenia wartosci bezwzglednej (punkt moze
  // znajdowac sie po ujemnej stronie plaszczyzny, tym samym prowadzac
  // do uzyskania wyniku ujemnego podczas obliczania iloczynu skalarnego)
  // zwroc obliczona wartosc, ktora okresla odleglosc punktu od plaszczyzny
  return fabs( length );
}

//   Wynikiem dzialania ponizszej funkcji jest wartosc okreslajaca punkt
// w przestrzeni, dla ktorego wynik rownania plaszczyzny jest rowny zeru.
// Stad mozna wnioskowac, iz znajduje sie on na plaszczyznie. Oprocz tego
// cecha charakterystyczna wyliczonego punktu jest wartosc okreslajaca
// odleglosc zawarta miedzy nim, a punktem podanym w drugim argumencie
// wywolania funkcji, ktora jest tozsama z najkrotsza dlugoscia zawarta
// pomiedzy punktami. Konkluzja: funkcja oblicza punkt, ktory mozna
// traktowac, jako rzut punktu drugiego argumentu na plaszczyzne
// okreslona pierwszym parametrem.
//   Przy obliczaniu najkrotszej wartosci dlugosci od punktu do
// plaszczyzny nie nalezy poslugiwac sie wartoscia bezwzgledna, poniewaz
// w tym wypadku, znak wyliczonej wartosci jest brany pod uwage.
//
// nearest_point =
//   point - ( (point - plane_point) . plane_normal ) * plane_normal =
//   point - distance * plane_normal
//
// source: "Perelki programowania gier" tom II str. 220
//-------------------------------------------------------------------------------

void PlaneUtil
::nearestPoint( const Plane&    plane,
                const Vector3&  point,
                Vector3*        outPoint ) 
{
  assert( outPoint != 0 );

  // nalezy obliczyc wektor zawarty pomiedzy przekazanym punktem,
  // a punktem nalezacym do plaszczyzny
  Vector3Util::sub( point, plane.p0(), outPoint );

  // pozniej wykonujemy rzut (iloczyn skalarny) obliczonego wektora
  // na jednostkowy wektor normalny plaszczyzny, tym samym
  // otrzymujemy wartosc dlugosci jego rzutu
  Real castLength = Vector3Util::dot( plane.n(), *outPoint );

  // nastepnie mnozymy otrzymana dlugosc przez normalna powierzchni
  // i otrzymujemy wektor, ktory jest do niej prostopadly, przy czym
  // jego dlugosc jest proporcjonalna do najkrotszej odleglosci pomiedzy
  // punktem okreslonym w drugim argumencie funkcji, a powierzchnia
  Vector3Util::mul( plane.n(), castLength, outPoint );

  // odejmujemy obliczony wektor od punktu drugiego argumentu tym samym
  // otrzymujemy punkt lezacy na plaszczyznie
  Vector3Util::sub( point, *outPoint, outPoint );
}

// funkcja zwraca informacje na temat zaistnienia przeciecia pomiedzy dana
// parametryczna prosta, a plaszczyzna. schemat dzialania:
// oblicza iloczyny skalarne osobno dla punktow poczatkowego i koncowego danej
// parametrycznej prostej z plaszczyzna ( sprawdza ich polozenie wzgledem 
// plaszczyzny ) i zwraca wartosc true, jesli iloczyny sa wartosciami 
// przeciwnymi. implikuje to twierdzenie, iz oba punkty znajduja sie po dwoch 
// roznych stronach plaszczyzny ( wektory poprowadzone od punktu znajdujacego 
// < nalezacego, spelniajacego rownanie > sie na plaszczyznie do punktow
// poczatkowego oraz koncowego parametrycznej prostej tworza kat ostry oraz
// rozwarty z wektorem normalnym powierzchni )
//-------------------------------------------------------------------------------

bool PlaneUtil
::isIntersect( const Plane&       plane, 
               const ParamLine3&  line ) 
{
  // obliczamy dwa wektory, ktore beda potrzebne do obliczenia iloczynu
  // skalarnego z wektorem normalnym plaszczyzny
  Vector3 v1( line.p0() - plane.p0() );
  Vector3 v2( ParamLine3Util::endPoint( line ) - plane.p0() );
  
  // oddzielnie obliczamy iloczyny skalarne wektorow poprowadzonych od punktu 
  // znajdujacego sie na plaszczyznie osobno do punktow poczatkowego i koncowego 
  // prostej. jezeli oba iloczyny zwroca wartosci dodatnie lub ujemne, wowczas 
  // bedzie to oznaczac, iz leza one po jednej stronie plaszczyzny, czyli 
  // jej nie przecinaja, funkcja wowoczas zwraca falsz, w przeciwnym wypadku 
  // funkcja zwraca prawde 
  //
  // simple boolean logic:
  // ( - * - = + )  ( + * + = + )  ( - * + = - ) 
  // ( 0 && 0 = 0 ) ( 1 && 1 = 1 ) ( 0 && 1 = 0 )
  return !( Vector3Util::dot( v1, plane.n() ) * 
    Vector3Util::dot( v2, plane.n() ) );
}

//  oblicza punkt przeciecia ( o ile taki istnieje ) prostej z plaszczyzna
//  przekazana przez argument aktualny. jezeli prosta jest rownolegla do
//  plaszczyzny nalezy sprawdzic czy punkt ktory nalezy do prostej spelnia
//  rownanie plaszczyzny. jesli wynik sprawdzenia bedzie pozytywny, wowczas
//  oznacza to iz prosta zawiera sie w plaszczyznie. funkcja zmienia wartosc
//  _t prostej, jezeli punkt jej przeciecia z plaszczyzna istnieje
//  ( nie odcinka ) z plaszczyzna
//
//  Rownanie plaszczyzny:
//
//    n . ( p - p0 ) = 0
//    nxpx + nypy + nzpz - nxp0x - nyp0y - nzp0z = 0
//
//  Rownanie parametryczne prostej:
//
//    l = l0 + v * t
//    lx = l0x + vx * t    ly = l0y + vy * t    lz = l0z + vz * t
//
//  nx * lx + ny * ly + nz * lz - nxp0x - nyp0y - nzp0z = 0
//  ( n . l ) = ( n . p0 )
//  t( nxvx + nyvy + nzvz ) + ( n . l0 ) = ( n . p0 )
//  t = ( n . p0 ) - ( n . l0 ) / ( n . v )
//
//-------------------------------------------------------------------------------

/** \todo sprawdzic mechanizm dzialania */
PlaneUtil::EIntersectResult PlaneUtil
::intersect( const Plane&  plane,
             ParamLine3*   line ) 
{
  assert( line != 0 );

  // jezeli iloczyn skalarny odcinka z normalna plaszczyzny zwroci wartosc bliska
  // zeru, wowczas kat pomiedzy wektorami jest bliski 90 stopni. nalezy sprawdzic
  // czy prosta lezy na plaszczyznie, lub czy jest do niej rownolegla
  if( Vector3Util::dot( line->v(), plane.n() ) < 1e-5 )
    // gdy punkt nalezacy do odcinka spelnia rownanie plaszczyzny, wowczas
    // odcinek, oraz prosta ktora wyznacza, lezy na plaszczyznie ( EIR_ON )
    return ( pointSide( plane, line->p0() ) == EPS_ON ) ? EIR_ON : EIR_PARALLEL;

/* metoda silowa zaimplementowana ponizej
  line->setT( 
    ( -plane.n().x() * line->p0().x() - 
    plane.n().y() * line->p0().y() - 
    plane.n().z() * line->p0().z() +
    plane.n().x() * plane.p0().x() + 
    plane.n().y() * plane.p0().y() + 
    plane.n().z() * plane.p0().z() ) / 
    ( plane.n().x() * line->v().x() + 
    plane.n().y() * line->v().y() + 
    plane.n().z() * line->v().z() ) 
  ); 
*/

  // inna reprezentacja powyzszego zapisu
  line->setT( 
    ( Vector3Util::dot( plane.n(), plane.p0() ) - 
    Vector3Util::dot( plane.n(), line->p0() ) ) / 
    Vector3Util::dot( plane.n(), line->v() ) 
  );
  
  return ( line->t() >= 0 && line->t() <= 1 ) ? EIR_IN : EIR_OUT;
}

//  oblicza punkt przeciecia odcinka wyznaczonego przez 2 punkty z plaszczyzna.
//  gdy funkcja zwroci falsz, oznaczac to bedzie, iz odcinek jest rownolegly
//  do plaszczyzny, a warosc p bedzie pozostawiona. dla prawdy zmienna p bedzie
//  wartoscia punktu przeciecia prostej z plaszczyzna
//
//                     ( line.p0 - plane.p0 ) . plane.n 
//  point = line.p0 + ---------------------------------- * ( line.p1 - line.p0 )
//                      ( line.p0 - line.p1 ) . plane.n
//
//  line.p1 = line.p0 + line.v ( funkcja ParamLine3Util::endPoint )
//
//  perelki programowania gier tom II str. 221
//-------------------------------------------------------------------------------

/** \todo sprawdzic dzialanie funkcji */
bool PlaneUtil
::computePoint( const Plane&       plane,
                const ParamLine3&  line, 
                Vector3*           point ) 
{
  assert( point != 0 );

  // wektor wyznaczajacy odcinek danej prostej
  Vector3 section( line.v() );
  
  // oblicza rzut ( dlugosc ) wektora skierowanego ( odcinka "section" ) 
  // wzgledem normalnej plaszczyzny, dzieki wlasnosci wektora normalnego 
  // plaszczyzny dla ktorego | n | == 1. ( ponizsze rownanie jest tozsame
  // z mianownikiem rownania opisowego )
  Real invSectCastLen = Vector3Util::dot( -section, plane.n() );

  // jelsli iloczyn skalarny bliski zeru, wowczas prosta zawierajaca odcinek
  // jest rownolegla do plaszczyzny - brak punktu przeciecia ( odcinek 
  // wzgledem wektora normalnego plaszczyzny tworzy kat 90 stopni )
  if( invSectCastLen < 1e-5 )
    return false;

  // dzieki temu wektorowi bedziemy mogli obliczyc najblizsza odleglosc
  // dzielaca punkt poczatkowy odcinka do punktu lezacego na plaszczyznie
  Vector3 vecFromLineToPlane( plane.p0() - line.p0() );

  // obliczamy stosunek dlugosci ( rzut na wektor normalny ) wektora
  // line.p0 -->> plane.p0 ( plane.p0 - line.p0 ) do dlugosci rzutu 
  // wektora -line.v ( invertSectionLen ) zrzutowanego na wektor normalny, 
  // ( nearestDistance( plane, line.p0 ) / invertSectionLen )
  Real R = Vector3Util::dot( vecFromLineToPlane, plane.n() ) / invSectCastLen;

  *point = line.p0() + R * -section;

  return true;
}

// oblicza punkt odbicia przy zalozeniu, iz prosta przecina plaszczyzne
//
// src: perelki programowania gier tom II str. 221
//-------------------------------------------------------------------------------

Vector3 PlaneUtil
::computeReflectPoint( const Plane&       plane, 
                       const ParamLine3&  line ) 
{
  // funkcja brana pod uwage jedynie podczas konstruowania oprogramowania
  assert( isIntersect( plane, line ) == true );
  
  const Vector3  doubledPlaneNormal = plane.n() * 2;
  const Vector3  lineEndPoint = ParamLine3Util::endPoint( line );

  return lineEndPoint + doubledPlaneNormal * 
    Vector3Util::dot( ( plane.p0() - lineEndPoint ), plane.n() ); 
}


} // namespace Math
} // namespace Gcad

#endif
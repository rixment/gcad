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

#ifndef _GCAD_PARAMLINE2UTIL_H_
#define _GCAD_PARAMLINE2UTIL_H_

#include "GcadAssertion.h"
#include "GcadParamLine2.h"
#include "GcadVector2Util.h"
#include <cmath>

namespace Gcad {
namespace Math {

/** 
  @brief
    Zestaw funkcji dokonujacych operacji na 
    prostych parametrycznych na plaszczyznie
*/
struct ParamLine2Util {

  /** 
    @brief 
      Synonim typu, ktego wartosc jest zwracana przez 
      funkcje wyliczajaca przeciecia prostych parametrycznych
      (ParamLine2Util::intersect)
  */
  enum EIntersectResult {
    EQUAL,    /**< Odcinki leza na tej samej prostej */
    PARALLEL, /**< Odcinki sa rownolegle wzgledem siebie */
    IN,       /**< Punkt przeciecia nastepuje wewnatrz odcinkow */
    OUT       /**< Punkt przeciecia nastepuje na zewnatrz odcinkow
                   (przeciecie prostych) */
  };
  
  /** 
    @brief 
      Obliczenie wartosci nachylenia prostej 
      
      Funkcja oblicza wspolczynnik nachylenia prostej.
      Poniewaz wektor v_ jest wektorem wyznaczajacym orientacje 
      prostej parametrycznej (jej kierunek), wiec nachylenie mozna 
      obliczyc dzielac jego skladowa y_ przez x_
     
      m = dy / dx
  */
  template< typename REAL >
  static REAL 
    bend( const ParamLine2<REAL>&  line );

  /** 
    @brief 
      Oblicza dlugosc odcinka
  */
  template< typename REAL >
  static REAL 
    length( const ParamLine2<REAL>&  line );
  
  /** 
    @brief 
      Sprawdza czy punkt nalezy do prostej
      
      Zmienne:
       - x
       - y

      Czynniki wyznaczajace prosta:
       - m
       - x0
       - y0

      Wzory:
       - ( y - y0 ) = m( x - x0 )
       - m * ( x - x0 ) - y + y0 = 0

    @return
      Funkcja zwraca prawde, gdy punkt przeslany w postaci argumentu
      nalezy do odcinka
  */
  template< typename REAL >
  static bool 
    includePoint( const ParamLine2<REAL>&  line,
                  const Vector2<REAL>&     point );

  /** 
    @brief Zwraca punkt koncowy odcinka
  */
  template< typename REAL >
  static Vector2<REAL> 
    endPoint( const ParamLine2<REAL>& line );
  
  /** 
    @brief 
      Zwraca punkt nalezacy do prostej okreslony przez wartosc argumentu t

      Zwraca punkt nalezacy do odcinka (jesli t bedzie zawierac sie 
      w przedziale [0, 1]), lub punkt nalezacy do prostej (gdy t != [0, 1])
  */
  template< typename REAL >
  static Vector2<REAL> 
    computePoint( const ParamLine2<REAL>&  line,
                  REAL                     t );

  /** 
    @brief Obliczenie wartosci przeciecia prostych
  */
  template< typename REAL >
  static EIntersectResult 
    intersect( const ParamLine2<REAL>&  k,
               const ParamLine2<REAL>&  l,
               Vector2<REAL>*           out );
  
  /* 
    @brief 
      Oblicza punkt przeciecia dwoch prostych.

    @remark 
      Oblicza punkt przeciecia dwoch prostych (o ile tak owe 
      mialo miejsce) i ustawia odpowiednio czynniki t obu obiektow. 
      Wartosc zwrocona informuje klienta, czy nastapilo, oraz gdzie 
      nastapilo przeciecie
  */
  template< typename REAL >
  static EIntersectResult 
    intersect( ParamLine2<REAL>*  k,
               ParamLine2<REAL>*  l );
 
 private:
   // Nie zaimplementowane
   ParamLine2Util();
   ParamLine2Util( const ParamLine2Util& );
   ParamLine2Util& operator =( const ParamLine2Util& );

};


template<typename REAL>
REAL 
ParamLine2Util
::bend( const ParamLine2<REAL>&  line )
{
  const REAL NEAR_ZERO = 1e5;
  const REAL x = line.v().x() == static_cast<REAL>(0) ? NEAR_ZERO : line.v().x();

  return line.v().y() / x;
}

template<typename REAL>
REAL 
ParamLine2Util
::length( const ParamLine2<REAL>&  line ) 
{
  return Vector2Util::length( line.v() );
}

template<typename REAL>
bool 
ParamLine2Util
::includePoint( const ParamLine2<REAL>&  line,
                const Vector2<REAL>&     point ) 
{
  // obliczamy rownanie prostej dla zmiennych przeslanego punktu
  REAL result =
    bend( line ) * ( point.x() - line.p0().x() ) - point.y() + line.p0().y();
  
  // jesli wartosc jest bliska zeru, to punkt nalezy do prostej
  if( result < 1e-5 ) {
    REAL  p0x, pv;
    // zmieniamy kolejnosc komponentu x_, jesli punkt poczatkowy prostej
    // p0_ ( wartosc skladowej x ) jest wiekszy od punktu koncowego
    // endPoint() ( komponentu x )
    if( line.p0().x() > line.p0().x() + line.v().x() ) {
      p0x = line.p0().x() + line.v().x();
      pv = line.p0().x();
    } else {
      p0x = line.p0().x();
      pv = line.p0().x() + line.v().x();
    }
    // sprawdzamy czy point nalezy do przedzialu
    // wyznaczanym przez odcinek prostej
    return point.x() >= p0x && point.x() <= pv;
  } else // punkt nie spelnia rownania prostej
    return false;
}

template<typename REAL>
Vector2<REAL> 
ParamLine2Util
::endPoint( const ParamLine2<REAL>&  line ) 
{
  return line.p0() + line.v();
}

template<typename REAL>
Vector2<REAL>
ParamLine2Util
::computePoint( const ParamLine2<REAL>&  line,
                REAL                     t ) 
{
  return line.p0() + line.v() * t;
}

//
//       p0y_2 - m_2 * p0x_2 - p0y_1 + m_1 * p0x_1
//  x = -------------------------------------------
//                      m_1 - m_2
//
//
//  y = m_1 * ( x - p0x_1 ) + p0y_1
//
//-------------------------------------------------------------------------------
template<typename REAL>
ParamLine2Util::EIntersectResult 
ParamLine2Util
::intersect( const ParamLine2<REAL>&  k,
             const ParamLine2<REAL>&  l,
             Vector2<REAL>*           out ) 
{
  Utilities::assertion( out != 0, "Bledna wartosc argumentu out!" );
  // sprawdzamy czy wspolczynniki nachylenia prostych sa prawie rowne
  // ( wartosc wyrazenia ich roznicy jest bliska zeru )
  // ( obliczenia na liczbach zmiennoprzecinkowych nie sa dokladne i wprowadzaja
  // bledy, poniewaz nie moga one przechowywac nieskonczenie duzych
  // wielkosci ). jezeli calosc jest prawda oznacza to, ze proste sa do siebie
  // rownolegle. wartosc jest porownywana z bardzo mala liczba, ze wzgledu na
  // wspomniane bledy numeryczne. fabs zwraca wartosc bezwzgledna ( instrukcja
  // FABS FPU
  //
  //  dy1   dy2
  //  --- = ---       dy1 * dx2 = dy2 * dx1         dy1 * dx2 - dy2 * dx1 = 0
  //  dx1   dx2

  // wspolczynniki nachylenia prostych
  REAL m1 = bend( k );
  REAL m2 = bend( l );

  // proste posiadaja rowny wspolczynnik nachylenia. pytania sa nastepujace:
  // czy wyznaczaja ta sama linie? czy sa rownolegle?
  if( ( std::fabs( m1 - m2 ) ) < 1e-5 ) {
    // calkiem mozliwe, ze obie proste parametryczne wyznaczaja ta sama linie,
    // wiec przyjmujemy dla zmiennej x jednego rownania arbitalnie wybrana
    // wartosc rowna 1 ( x=1 ) i obliczamy wartosc zmiennej y. Nastepnie
    // wyliczona wartosc y podstawiamy do drugiego rownania prostej i sprawdzamy
    // czy wynik dzialania bedzie rowny x=1. Bedzie to implikowac twierdzenie, 
    // iz proste sa tozsame

    // y = m * ( 1 - x0 ) + y0
    REAL y = m1 * ( 1 - l.p0().x() ) + l.p0().y();
    REAL x = ( y - k.p0().y() ) / m2 + k.p0().x();
    
    return ( 1 - x < 1e-5 ) ? EQUAL : PARALLEL;
  }

  // proste najwyrazniej nie sa rownolegle, wiec przystepujemy 
  // do obliczenia miejsca przeciecia
  REAL  x_out, y_out;

  x_out = ( l.p0().y() - m2 * l.p0().x() - k.p0().y() + m1 * k.p0().x() ) / 
    ( m1 - m2 );

  y_out = m1 * ( x_out -  k.p0().x() ) + k.p0().y();

  out->setX( x_out );
  out->setY( y_out );

  // na koniec sprawdzamy, czy punkt przeciecia prostych nastapil wewnatrz
  // obu odcinkow
  return ( includePoint( k, *out ) && includePoint( l, *out ) ) ? IN : OUT;
}

//  funkcja oblicza punkt przeciecia dwoch prostych ( o ile takie mialo miejsce )
//  i ustawia odpowiednio zmienne t_ obu obiektow. wartosc zwrocona informuje
//  czy przeciecie takie mialo miejsce przy zalozeniu iz nastapilo w obrebie
//  odcinkow
//
//        vx_1 ( p0y_1 - p0y_2 ) + vy_1 * ( p0x_2 - p0x_1 )
//  t2 = ---------------------------------------------------
//                    vx_1 * vy_2 - vy_1 * vx_2
//
//
//        p0x_2 - p0x_1 + vx_2 * t2
//  t1 = ---------------------------
//                  vx_1
//
//-------------------------------------------------------------------------------

template<typename REAL>
ParamLine2Util::EIntersectResult 
ParamLine2Util
::intersect( ParamLine2<REAL>*  k,
             ParamLine2<REAL>*  l ) 
{
  Utilities::assertion( k != 0, "Bledna wartosc argumentu k!" );
  Utilities::assertion( l != 0, "Bledna wartosc argumentu l!" );

  REAL m1 = bend( *k );
  REAL m2 = bend( *l );

  // sprawdzamy czy proste nie sa przypadkiem rownolegle. gdy twierdzenie jest
  // prawdziwe, wowczas nie podejmujemy zadnych dzialan na czynnikach t
  // obu prostych
  if( std::fabs( m1 - m2 ) < 1e-5 )
    return PARALLEL;

  REAL t1, t2;
  // obliczamy warosci t, dla obu prostych w punkcie przeciecia ( calkiem
  // mozliwe ze przeciecie nastapilo poza obszarem obu odcinkow, lub jednego
  // z nich )
  t2 = ( k->v().x() * ( k->p0().y() - l->p0().y() ) + 
    k->v().y() * ( l->p0().x() - k->p0().x() ) ) / 
    ( k->v().x() * l->v().y() - k->v().y() * l->v().x() );
         
  t1 = ( l->p0().x() - k->p0().x() + l->v().x() * t2 ) / k->v().x();

  // dokonujemy zmiany na wartosci czynnika t obu prostych
  k->setT( t1 );
  l->setT( t2 );

  // zwracamy informacje o miejscu przeciecia
  return ( t1 <= 1 && t1 >= 1 && t2 <= 1 && t2 >= 1 ) ? IN : OUT;
}


} // namespace Math
} // namespace Gcad

#endif
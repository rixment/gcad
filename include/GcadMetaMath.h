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

#ifndef _GCAD_METAMATH_H_
#define _GCAD_METAMATH_H_

namespace Gcad {
namespace Math {

/**
  @brief
    Konkretyzacja szablonu pozwala wyliczyc juz w czasie kompilacji
    wartosc funkcji Fibonacciego o zadanej wartosci zmiennej wejsciowej

  @code
    #include "GcadMetaMath.h"

    using namespace Gcad::Math;

    int main()
    {
      // Wyliczenie na etapie kompilacji wartosci funkcji Fibonacciego
      // wykonanych dla zadanych wartosci
      const int fibonacciValues[] = { 
        Fibonacci<10u>::VALUE,  // Jawne wywolanie konkretyzacji
        Fibonacci<7u>::VALUE    // szablonu ciagu Fibonacciego
      };
    }
  @endcode
*/
template< unsigned int N >
struct Fibonacci {
  enum {
    VALUE = Fibonacci< N - 1 >::VALUE + Fibonacci< N - 2 >::VALUE
  };
};

// Specjalizacje szablonu determinujace poczatkowe wartosci 
// umozliwiajace wydostanie sie z rekurencyjnych wywolan, ktorych
// rezultatem jest wartosc ciagu Fibonacciego

template<>
struct Fibonacci< 0 > {
  enum {
    VALUE = 0
  };
};

template<>
struct Fibonacci< 1 > {
  enum {
    VALUE = 1
  };
};

/**
  @brief
    Obliczenie wartosci funkcji silnia wykonywane w trakcie kompilacji
    za posrednictwem wlasciwosci zwiazanych z szablonami

  @code
    #include "GcadTemplateFunctions.h"

    using namespace Gcad::Math;

    int main()
    {
      const int fact = Fact<5u>::VALUE; // Obliczenie wartosci funkcji 5!
    }
  @endcode
*/
template< unsigned int N >
struct Fact {
  enum {
    VALUE = N * Fact< N - 1 >::VALUE
  };
};

// Specjalizacja determinujaca warunek 
// zakonczenia rekurencyjnych wywolan

template<>
struct Fact< 1 > {
  enum {
    VALUE = 1
  };
};

} // namespace Math
} // namespace Gcad

#endif

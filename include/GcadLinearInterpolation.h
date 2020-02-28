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

#ifndef _GCAD_LINEARINTERPOLATION_H_
#define _GCAD_LINEARINTERPOLATION_H_

#include "GcadAssertion.h"

namespace Gcad {
namespace Math {

/**
  @brief
    Definicja funkcji interpolacji liniowej

  @code
    #include "GcadLinearInterpolation.h"
    #include <iostream>

    using namespace Gcad::Math;
    using namespace std;

    int main()
    {
      const int  FROM  = 0;
      const int  TO    = 10;
      const int  STEPS = 500;

      LinearInterpolation<double>  interpolate( FROM, TO, STEPS );
      while( !interpolate.isFinished() ) {
        interpolate.update();
        cout << "Step: " << interpolate.currentStep() <<
          " value: " << interpolate.value() << endl;
      }
    }
  @endcode
*/
template< typename REAL >
class LinearInterpolation {
 public:
   /**
     @brief 
       Konstruktor wyliczajacy wartosc krokowa kolejnych
       przyblizen interpolacyjnych
   */
   LinearInterpolation( REAL  from,
                        REAL  to,
                        int   steps );

   /**
     @brief
       Zresetowanie biezacego stanu, poprzez ustawienie nowych
       wartosci liniowej interpolacji
   */
   void reset( REAL from,
               REAL to,
               int  steps );

   /**
     @brief 
       Wartosc determinujaca aktualnie przeprowadzany krok iteracji
   */
   int currentStep() const;

   /**
     @brief
       Wykonanie kroku iteracyjnego, ktorego wynikiem jest
       zwiekszenie interpolowanej wartosci
   */
   void update();

   /**
     @brief
       Aktualnia wartosc interpolacji
   */
   REAL value() const;

   /**
     @brief
       Wykonanie testu, majacego na celu okreslenie zakonczenia
       przetwarzania funkcji interpolacji
   */
   bool isFinished() const;

 private:
   REAL  start_;
   REAL  finish_;
   REAL  currentStep_;
   REAL  stepFactor_;
};


//
template<typename REAL>
void LinearInterpolation
::reset( REAL from,
         REAL to,
         int  steps )
{
  Utilities::assertion( steps > 0, "Zadana wartosc determinujaca ilosc wykonywanych"
    "interpolacyjnych iteracji musi byc wieksza od zera!" );

  start_= currentStep_ = from;
  finish_ = to;  
  stepFactor_ = ( to - from ) / steps;
}

//
template<typename REAL>
LinearInterpolation
::LinearInterpolation( REAL  from,
                       REAL  to,
                       int   steps )
{
  reset( from, to, steps );
}

//
template<typename REAL>
int 
LinearInterpolation
::currentStep() const
{
  return currentStep_ / stepFactor_;
}
    
//
template<typename REAL>
void 
LinearInterpolation
::update()
{
  currentStep_ += stepFactor_;
}

//
template<typename REAL>
REAL 
LinearInterpolation
::value() const
{
  return currentStep_;
}

//
template<typename REAL>
bool 
LinearInterpolation
::isFinished() const
{
  return currentStep_ >= finish_;
}

} // namespace Math
} // namespace Gcad

#endif
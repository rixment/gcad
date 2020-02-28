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

#ifndef _GCAD_SINGLETON_H_
#define _GCAD_SINGLETON_H_

namespace Gcad {
namespace Utilities {

/** 
  @brief
    Klasa implementujaca zachowanie tozsame z wzorcem Singleton

    Klasa implementujaca zachowanie tozsame z wzorzem Singleton.
    Encja ta nalezy do zbioru wzorcow okreslanych mianem strukturalne. 
    Dzieki skorzystaniu z udostepnionej wlasciwosci wzorca Singleton, 
    mamy do dyspozycji jedna instancje, ktora semantycznie podobna 
    jest do zmiennej globalnej. Korzyscia plynaca z uzycia wzorca 
    Singleton, zamiast obiektu globalnego jest automatyczne utworzenie 
    instancji, podczas jej pierwszego uzycia, co jest rownoznaczne ze 
    stwierdzeniem, iz obiekt nie zostanie uzyty w sytuacji, w ktorej 
    nie zostal jeszcze poprawnie zbudowany, zainicjalizowany 
    (nie ruszyl do pracy jego konstruktor)
  
  @remark 
    Z definicji metod pobierajacych instancje mozna zauwazyc, iz
    obiekt uzywajacy wzorca do uzyskania zachowania pojedynczej instancji
    powinien posiadac konstruktor domyslny
  
  @param 
    T Klasa adaptujaca zachowanie Singleton

  @code
    #include "GcadSingleton.h"
    #include <iostream>
    
    using namespace Gcad::Utilities;
    using namespace std;

    class SimpleSingleton : public Singleton< SimpleSingleton > {
     public:
       void print() {
         cout << "SimpleSingleton::print()" << endl;
       }
    };

    void main()
    {
      SimpleSingleton::getSingleton().print();
    }
  @endcode
*/
template< typename T > class Singleton {
 public:
   /** 
     @brief 
       Udostepnienie instancji jedynego klonu 
   */
   static T& getSingleton() {
     return oneInstance_ == 0 ? *( oneInstance_ = new T ) : *oneInstance_;
   }

   /** 
     @brief 
       Udostepnienie instancji jedynego klonu 
   */
   static T* getSingletonPtr() {
     return oneInstance_ == 0 ? ( oneInstance_ = new T ) : oneInstance_;
   }

 protected:
   ~Singleton() {
     delete oneInstance_;
     oneInstance_ = 0;
   }

   Singleton() {}

 private:
   // Moze istniec jedynie pojedyncza instancja obiektu 
   // typu wydedukowanego dla danego szablonu
   static T*  oneInstance_;

   // Nie zaimplementowane
   Singleton( const Singleton& );
   Singleton& operator =( const Singleton& );
};


//
template< typename T > 
T* 
Singleton< T >
::oneInstance_ = 0;

} // namespace Utilities
} // namespace Gcad

#endif
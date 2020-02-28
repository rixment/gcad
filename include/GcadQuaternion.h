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

#ifndef _GCAD_QUATERNION_H_
#define _GCAD_QUATERNION_H_

#include "GcadAssertion.h"
#include "GcadMatrix.h"
#include "GcadVector3.h"
#include "GcadVector3Util.h"

namespace Gcad {
namespace Math {

/**
  @brief
    Implementacja kwaterniona bazujaca na tresciach zawartych w publikacjach
    - "Focus On 3D Models" Evan'a Pipho
    - Perelki programowania gier Tom I
*/
template<typename REAL>
class Quaternion {
 public:
   typedef Vector3<REAL> Vector3;

   Quaternion()
     : w_(static_cast<REAL>(0))
   {}

   Quaternion(const Vector3& v, REAL w)
     : v_(v)
     , w_(w)
   {}

   void setV(const Vector3& v) { v_ = v; }
   void setX(REAL x) { v_.setX(x); }
   void setY(REAL y) { v_.setY(y); }
   void setZ(REAL z) { v_.setZ(z); }
   void setW(REAL w) { w_ = w; }

   Vector3 v() const { return v_; }
   REAL    x() const { return v_.x(); }
   REAL    y() const { return v_.y(); }
   REAL    z() const { return v_.z(); }
   REAL    w() const { return w_; }
 
 private:
   Vector3 v_; /**< @brief Komponent wektorowy kwaterniona */
   REAL    w_; /**< @brief Czesc skalarna kwaterniona */
};

//! @brief Operacja porownania kwaternionow
template<typename REAL>
bool
operator ==(const Quaternion<REAL>& lhs,
            const Quaternion<REAL>& rhs)
{
  return lhs.w() == rhs.w() &&
    lhs.v() == rhs.v();
}

//! @brief Operacja porownania kwaternionow
template<typename REAL>
bool
operator !=(const Quaternion<REAL>& lhs,
            const Quaternion<REAL>& rhs)
{
  return !(lhs==rhs);
}

/**
  @brief 
    Operacja negacji kwaterniona
  
  @remark 
    Kwaternion, oraz jego zanegowany odpowiednik reprezentuja te same
    katowe przemieszczenie
*/
template<typename REAL>
Quaternion<REAL>
operator -(const Quaternion<REAL>& q)
{
  return Quaternion<REAL>(-q.v(), -q.w());
}

//! @brief Operacja dodawania kwaternionow
template<typename REAL>
Quaternion<REAL>
operator +(const Quaternion<REAL>& p,
           const Quaternion<REAL>& q)
{
  return Quaternion<REAL>(p.v() + q.v(), p.w() + q.w());
}

//! @brief Operacja odejmowania kwaternionow
template<typename REAL>
Quaternion<REAL>
operator -(const Quaternion<REAL>& p,
           const Quaternion<REAL>& q)
{
  return Quaternion<REAL>(p.v() - q.v(), p.w() - q.w());
}

//! @brief Operacja mnozenia kwaterniona przez skalar
template<typename REAL>
Quaternion<REAL>
operator *(REAL scalar,
           const Quaternion<REAL>& p)
{
  return Quaternion<REAL>(scalar * p.v(), scalar * p.w());
}

template<typename REAL>
inline 
Quaternion<REAL>
operator *(const Quaternion<REAL>& p,
           REAL scalar)
{
  return scalar * p;
}

//! @brief Operacja mnozenia kwaternionow
//! @see 3D Math Primier for Graphics and Game Development, page 168
template<typename REAL>
Quaternion<REAL>
operator *(const Quaternion<REAL>& p,
           const Quaternion<REAL>& q)
{
  Vector3<REAL> qXp;
  Vector3Util::cross(q.v(), p.v(), &qXp);
  return Quaternion<REAL>(
    q.w() * p.v() + p.w() * q.v() + qXp,
    p.w() * q.w() - Vector3Util::dot(q.v(), p.v())
  );
}

/**
  @brief 
    Zestaw funkcji pracujacych na kwaternionach
*/
struct QuaternionUtil {
  //! @brief Operacja utworzenia kwaterniona na podstawie przekazanych wartosci
  template<typename REAL>
  static
  Quaternion<REAL>
  makeQuaternion(const Vector3<REAL>& rotateAxis,
                 REAL angle)
  {
    REAL S = sin(angle / 2);
    return Quaternion<REAL>( 
      Vector3<REAL>(S * rotateAxis.x(), S * rotateAxis.y(), S * rotateAxis.z()),
      cos(angle / 2)
    );
  }

  //! @brief Utworzenie kwaterniona jednostkowego
  template<typename REAL>
  static
  Quaternion<REAL>
  makeIdentity()
  {
    return Quaternion<REAL>(Vector3<REAL>(), 0);
  }

  //! Wyciagniecie skladowej kwaterniona determinujacej wektor obrotu
  template<typename REAL>
  static
  Vector3<REAL>
  extractAxis(const Quaternion<REAL>& p)
  {
    REAL SA = static_cast<REAL>( sqrt(1.0 - p.w()*p.w()) );
    return Vector3<REAL>( p.x()/SA, p.y()/SA, p.z()/SA );
  }

  //! Wyciagniecie skladowej kwaterniona determinujacej kat wokol wektora obrotu
  template<typename REAL>
  static
  REAL
  extractAngle(const Quaternion<REAL>& p)
  {
    return static_cast<REAL>( 2.0 * acos(p.w()) );
  }

  //! @brief Operacja obliczenia wielkosci kwaterniona
  template<typename REAL>
  static
  REAL
  magnitude(const Quaternion<REAL>& p)
  {
    const REAL POW_W = p.w() * p.w();
    return Vector3Util::length(p.v()) + 
      sqrt(static_cast<double>(POW_W));
  }

  //! @brief Operacja obliczenia kwaterniona jednostkowego
  template<typename REAL>
  static
  Quaternion<REAL>
  normalize(const Quaternion<REAL>& p)
  {
    const REAL MAGNITUDE = magnitude<REAL>(p);
    return Quaternion<REAL>(p.v() / MAGNITUDE, p.w() / MAGNITUDE);
  }

  //! @brief Iloczyn skalarny kwaternionow
  template<typename REAL>
  static
  REAL
  dot(const Quaternion<REAL>& lhs,
      const Quaternion<REAL>& rhs)
  {
    return Vector3Util::dot(lhs.v(), rhs.v()) + lhs.w() * rhs.w();
  }

  /**
    @brief 
      Operacja wyliczenia kwaterniona sprzezonego
  
    @remark 
      Kwaternion sprzezony kwaterniona jednostkowego jest rowny
      odwrotnosci kwaterniona:
        inverse(q) = conjugate(q) / magnitude(q)       
      dla magnitude(q) rownego 1:
        inverse(q) = conjugate(q)
  */
  template<typename REAL>
  static
  Quaternion<REAL>
  conjugate(const Quaternion<REAL>& p)
  {
    return Quaternion<REAL>(-p.v(), p.w());
  }

  //! @brief Wykonanie multiplikatywnej odwrotnosci kwaterniona 1/q
  template<typename REAL>
  static
  Quaternion<REAL>
  inverse(const Quaternion<REAL>& p)
  {
    return conjugate(p) * (1 / magnitude(p));
  }

  //! @brief Operacja obrotu kwaterniona, wzgledem drugiego kwaterniona
  template<typename REAL>
  static
  Quaternion<REAL>
  rotate(const Quaternion<REAL>& rotationAxis,
         const Quaternion<REAL>& beingRotated)
  {
    return rotationAxis * beingRotated * conjugate(rotationAxis);
  }

  //! @brief Operacja obrotu wektora, wzgledem kwaterniona
  template<typename REAL>
  static
  Vector3<REAL>
  rotate(const Quaternion<REAL>& rotationAxis,
         const Vector3<REAL>&    beingRotated)
  {
    return rotate( rotationAxis, Quaternion<REAL>(beingRotated, 0) ).v();
  }
  
  //! @brief Wykonanie liniowej interpolacji kwaternionow
  //! @param t Wspolczynnik interpolacji z przedzialu 0 =< t <= 1 
  template<typename REAL>
  static
  Quaternion<REAL>
  lerp(const Quaternion<REAL>& p,
       const Quaternion<REAL>& q,
       REAL t)
  {
    Utilities::assertion(t >= 0 && t <= 1,
      "Wartosc t liniowej interpolacji kwaternionow spoza zakresu!");
    return normalize<REAL>(t * (q - p) + p);
  }

  //! @brief Wykonanie sferycznej interpolacji kwaternionow
  //! @todo T ???
  template<typename REAL>
  static
  Quaternion<REAL>
  slerp(const Quaternion<REAL>& p,
        const Quaternion<REAL>& q,
        REAL t)
  {
    Utilities::assertion(t >= 0 && t <= 1,
      "Wartosc t sferycznej interpolacji kwaternionow spoza zakresu!");
    return p * sin(T * (1 - t)) + q * sin(T * t) / sin(T); //T==THETA
  }
};

/**
  @brief 
    Funkcja konwertujaca kwaternion do postaci macierzy (wynikowy
    typ macierzy powinien obslugiwac subskrypcje elementow)
*/
template<typename MATRIX,
         typename REAL>
void
quaternionToMatrix(const Quaternion<REAL>& p,
                   MATRIX* outMatrix3x3)
{
  Utilities::assertion(outMatrix3x3 != 0,
    "Bledny wskaznik obiektu macierzy!");

  MATRIX& matrix3x3 = *outMatrix3x3;

  const REAL POW_X = p.x() * p.x();
  const REAL POW_Y = p.y() * p.y(); 
  const REAL POW_Z = p.z() * p.z();
  const REAL POW_W = p.w() * p.w();

  matrix3x3[0][0] = POW_W + POW_X - POW_Y - POW_Z;
  matrix3x3[1][1] = POW_W - POW_X + POW_Y - POW_Z;
  matrix3x3[2][2] = POW_W - POW_X - POW_Y + POW_Z;

  const REAL TWO = static_cast<REAL>(2);
  const REAL X = p.x();
  const REAL Y = p.y();
  const REAL Z = p.z();
  const REAL W = p.w();

  matrix3x3[0][1] = TWO * X * Y - TWO * W * Z;
  matrix3x3[0][2] = TWO * X * Z + TWO * Y * W;

  matrix3x3[1][0] = TWO * X * Y + TWO * W * Z;
  matrix3x3[1][2] = TWO * Y * Z + TWO * W * X;

  matrix3x3[2][0] = TWO * X * Z - TWO * W * Y;
  matrix3x3[2][1] = TWO * Y * Z - TWO * W * X;
}

//! @brief Konwersja macierz->kwaternion
template<typename MATRIX,
         typename REAL>
void
matrixToQuaternion(const MATRIX& inMatrix3x3,
                   Quaternion<REAL>* p)
{
  Utilities::assertion(p != 0,
    "Bledny wskaznik obiektu kwaternionu!");

  REAL trace = inMatrix3x3[0][0] + inMatrix3x3[1][1] +inMatrix3x3[2][2];
  if(trace > 0) { 
    // mozna wykonac "szybkie obliczenie" (ang. instant calculation)
    REAL temp = 1 / ( 2 * static_cast<REAL>(sqrt(trace + 1)) );
    p->setW(static_cast<REAL>(0.25) / temp);
    p->setX( (inMatrix3x3[1][2] - inMatrix3x3[2][1]) * temp );
    p->setY( (inMatrix3x3[2][0] - inMatrix3x3[0][2]) * temp );
    p->setZ( (inMatrix3x3[0][1] - inMatrix3x3[1][0]) * temp );
  } 
  else {
    if( inMatrix3x3[0][0] > inMatrix3x3[1][1] &&
      inMatrix3x3[0][0] > inMatrix3x3[2][2] )
    {
      // Pierwszy element macierzy lezacy na przekatnej ma najwieksza wartosc
      REAL temp = 1 / (2 * static_cast<REAL>(
        sqrt(1 + inMatrix3x3[0][0] - inMatrix3x3[1][1] + inMatrix3x3[2][2]) ));
      p->setW(static_cast<REAL>(0.25) / temp);
      p->setX( (inMatrix3x3[1][0] - inMatrix3x3[0][1]) * temp );
      p->setY( (inMatrix3x3[0][2] - inMatrix3x3[2][0]) * temp );
      p->setZ( (inMatrix3x3[2][1] - inMatrix3x3[1][2]) * temp );
    }
    else 
    if( inMatrix3x3[1][1] > inMatrix3x3[2][2] ) 
    {
      // Drugi element macierzy...
      REAL temp = 1 / (static_cast<REAL>(
        sqrt(1 + inMatrix3x3[1][1] - inMatrix3x3[0][0] - inMatrix3x3[2][2]) ));
      p->setW( (inMatrix3x3[1][0] + inMatrix3x3[0][1]) * temp );
      p->setX(static_cast<REAL>(0.25) / temp);      
      p->setY( (inMatrix3x3[2][1] + inMatrix3x3[1][2]) * temp );
      p->setZ( (inMatrix3x3[0][2] - inMatrix3x3[2][0]) * temp );
    }
    else {
      // Trzeci element macierzy...
      REAL temp = 1 / (static_cast<REAL>(
        sqrt(1 + inMatrix3x3[2][2] - inMatrix3x3[0][0] - inMatrix3x3[1][1]) ));
      p->setW( (inMatrix3x3[0][2] + inMatrix3x3[2][0]) * temp );
      p->setX( (inMatrix3x3[2][1] + inMatrix3x3[1][2]) * temp );
      p->setY(static_cast<REAL>(0.25) / temp);
      p->setZ( (inMatrix3x3[1][0] - inMatrix3x3[0][1]) * temp );
    }
  }
}

//! @brief Konwersja katow Eulera do postaci kwaterniona
template<typename REAL>
void
eulerToQuaternion(REAL angleX,
                  REAL angleY,
                  REAL angleZ,
                  Quaternion<REAL> *outQuat)
{
  Utilities::assertion(outQuat != 0,
    "Obiekt kwaterniona nie ustawiony!");
  
  const REAL TWO = static_cast<REAL>(2);

  Quaternion<REAL> roll, pitch, yaw;
  
  roll.setW(cos(angleX / TWO));
  roll.setX(sin(angleX / TWO));

  pitch.setW(cos(angleY / TWO));
  pitch.setY(sin(angleY / TWO));

  yaw.setW(cos(angleZ / TWO));
  yaw.setZ(sin(angleZ / TWO));

  *outQuat = yaw * pitch * roll;
}

template<typename REAL>
inline 
void
eulerToQuaternion(const Vector3<REAL>& eulerAngles,
                  Quaternion<REAL> *outQuat)
{
  eulerToQuaternion(
    eulerAngles.x(), 
    eulerAngles.y(), 
    eulerAngles.z(),
    outQuat);
}

//! @brief Konwersja kwaterniona do postaci katow Eulera
template<typename REAL>
void
quaternionToEuler(const Quaternion<REAL>& p,
                  REAL* angleX,
                  REAL* angleY,
                  REAL* angleZ)
{
  Utilities::assertion(
    angleX != 0 &&
    angleY != 0 &&
    angleZ != 0,
    "Zmienna katu eulera nie ustawiona!");

  const REAL TWO = static_cast<REAL>(2);
  const REAL X = p.x();
  const REAL Y = p.y();
  const REAL Z = p.z();
  const REAL W = p.w();

  Matrix<3, 3, float> m;
  
  m[0][0] = W*W + X*X - Y*Y - Z*Z;
  m[1][0] = TWO * X * Y + TWO * W * Z;
  m[2][0] = TWO * X * Z - TWO * W * Y;
  m[2][1] = TWO * Y * X + TWO * W * X;
  m[2][2] = W*W - X*X - Y*Y + Z*Z;

  *angleX = atan(m[2][1]/m[2][2]); // roll
  *angleY = asin(-m[2][0]);        // pitch
  *angleZ = atan(m[1][0]/m[0][0]); // yaw
}

} // namespace Math
} // namespace Gcad

#endif

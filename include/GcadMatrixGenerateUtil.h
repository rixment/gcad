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

#ifndef _GCAD_MATRIXGENERATEUTIL_H_
#define _GCAD_MATRIXGENERATEUTIL_H_

#include "GcadAssertion.h"
#include "GcadVector2.h"
#include "GcadVector3.h"
#include "GcadMatrix.h"
#include <cmath>

namespace Gcad {
namespace Math {

struct MatrixGenerateUtil {

   template<typename T, int R, int C>
   static void 
     zero(Matrix<R,C,T>* matrix);

   /** 
     @brief
       Zaladowanie macierzy jednostkowej. Parametryzowany typ T powinien
       umozliwiac obsluge przypisania wartosci 1
   */
   template<typename T, int R>
   static void 
     identity(Matrix<R, R, T>* matrix);

   template<typename T>
   static void
     scale(const Vector2<T>& scaleFactors,
           Matrix<3, 3, T>* matrix);

   template<typename T>
   static void
     scale(const Vector3<T>& scaleFactors,
           Matrix<4, 4, T>* matrix);

   template<typename T>
   static void
     axisScale(const Vector3<T>& axis,
               T scaleFactor,
               Matrix<4, 4, T>* matrix);

   template<typename T> 
   static void 
     translate(const Vector2<T>& translationFactors,
               Matrix<3, 3, T>* matrix);

   template<typename T> 
   static void 
     translate(const Vector3<T>& translationFactors,
               Matrix<4, 4, T>* matrix);

   template<typename T>
   static void
     rotateX(T radiansians,
             Matrix<4, 4, T>* matrix);

   template<typename T>
   static void
     rotateY(T radiansians,
             Matrix<4, 4, T>* matrix);
   
   template<typename T>
   static void
     rotateZ(T radiansians,
             Matrix<4, 4, T>* matrix);

   template<typename T>
   static void
     axisRotate(const Vector3<T>& axis,
                T angle,
                Matrix<4, 4, T>* matrix);

   template<typename T>
   static void
     projection(T dFactor,
                Matrix<4, 4, T>* matrix);

 private:
   // nie zaimplementowane
   MatrixGenerateUtil();
   MatrixGenerateUtil( const MatrixGenerateUtil& );
   MatrixGenerateUtil& operator =( const MatrixGenerateUtil& );

};


template<typename T, int R, int C>
void 
MatrixGenerateUtil
::zero(Matrix<R,C,T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");
  for(int r=0; r<R; ++r)
    for(int c=0; c<C; ++c)
      (*matrix)[r][c] = static_cast<T>(0);
}

template<typename T, int R> 
void 
MatrixGenerateUtil
::identity(Matrix<R, R, T>* matrix) 
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");
  zero(matrix);
  for(int i=0; i<R; ++i)
    (*matrix)[i][i] = static_cast<T>(1);
}

template<typename T>
void
MatrixGenerateUtil
::scale(const Vector2<T>& scaleFactors,
        Matrix<3, 3, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[ 0 ][ 0 ] = scaleFactors.x();
  (*matrix)[ 1 ][ 1 ] = scaleFactors.y();
}

template<typename T>
void
MatrixGenerateUtil
::scale(const Vector3<T>& scaleFactors,
        Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[ 0 ][ 0 ] = scaleFactors.x();
  (*matrix)[ 1 ][ 1 ] = scaleFactors.y();
  (*matrix)[ 2 ][ 2 ] = scaleFactors.z();
}

template<typename T>
void
MatrixGenerateUtil
::axisScale(const Vector3<T>& axis,
            T scaleFactor,
            Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  Matrix<4, 4, T>& m = *matrix;

  m[0][0] = 1 + (scaleFactor - 1) * axis.x() * axis.x();
  m[0][1] = (scaleFactor - 1) * axis.x() * axis.y();
  m[0][2] = (scaleFactor - 1) * axis.x() * axis.z();

  m[1][0] = (scaleFactor - 1) * axis.x() * axis.y();
  m[1][1] = 1 + (scaleFactor - 1) * axis.y() * axis.y(); 
  m[1][2] = (scaleFactor - 1) * axis.y() * axis.z();

  m[2][0] = (scaleFactor - 1) * axis.x() * axis.z();
  m[2][1] = (scaleFactor - 1) * axis.y() * axis.z();
  m[2][2] = 1 + (scaleFactor - 1) * axis.z() * axis.z(); 
}

template<typename T> 
void 
MatrixGenerateUtil
::translate(const Vector2<T>& translationFactors,
            Matrix<3, 3, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[ 2 ][ 0 ] = translationFactors.x();
  (*matrix)[ 2 ][ 1 ] = translationFactors.y();
}

template<typename T> 
void 
MatrixGenerateUtil
::translate(const Vector3<T>& translationFactors,
            Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[ 2 ][ 0 ] = translationFactors.x();
  (*matrix)[ 2 ][ 1 ] = translationFactors.y();
  (*matrix)[ 2 ][ 2 ] = translationFactors.z();
}

template<typename T>
void
MatrixGenerateUtil
::rotateX(T radians,
          Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[1][1] = static_cast<T>( cos(radians) );
  (*matrix)[1][2] = static_cast<T>( sin(radians) );
  (*matrix)[2][1] = static_cast<T>(-sin(radians) );
  (*matrix)[2][2] = static_cast<T>( cos(radians) );
}

template<typename T>
void
MatrixGenerateUtil
::rotateY(T radians,
          Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[0][0] = static_cast<T>( cos(radians) );
  (*matrix)[0][2] = static_cast<T>(-sin(radians) );
  (*matrix)[2][0] = static_cast<T>( sin(radians) );
  (*matrix)[2][2] = static_cast<T>( cos(radians) );
}
   
template<typename T>
void
MatrixGenerateUtil
::rotateZ(T radiansians,
          Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  (*matrix)[0][0] = static_cast<T>( cos(radians) );
  (*matrix)[0][1] = static_cast<T>( sin(radians) );
  (*matrix)[1][0] = static_cast<T>(-sin(radians) );
  (*matrix)[1][1] = static_cast<T>( cos(radians) );
}

template<typename T>
void
MatrixGenerateUtil
::axisRotate(const Vector3<T>& axis,
             T angle,
             Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  Matrix<4, 4, T>& m = *matrix;
  
  m[0][0] = axis.x() * axis.x() * (1 - cos(angle)) + cos(angle);
  m[0][1] = axis.x() * axis.y() * (1 - cos(angle)) + axis.z() * sin(angle);
  m[0][2] = axis.x() * axis.z() * (1 - cos(angle)) - axis.y() * sin(angle);
  
  m[1][0] = axis.x() * axis.y() * (1 - cos(angle)) - axis.z() * sin(angle);
  m[1][1] = axis.y() * axis.y() * (1 - cos(angle)) + cos(angle); 
  m[1][2] = axis.y() * axis.z() * (1 - cos(angle)) + axis.x() * sin(angle);

  m[2][0] = axis.x() * axis.z() * (1 - cos(angle)) + axis.y() * sin(angle);
  m[2][1] = axis.y() * axis.z() * (1 - cos(angle)) - axis.x() * sin(angle);
  m[2][2] = axis.z() * axis.z() * (1 - cos(angle)) + cos(angle);
}

template<typename T>
void
MatrixGenerateUtil
::projection(T dFactor,
             Matrix<4, 4, T>* matrix)
{
  Utilities::assertion(matrix!=0,
    "Wskaznik macierzy nie ustawiony!");  
  identity(matrix);
  Matrix<4, 4, T>& m = *matrix;
  
  m[3][3] = 0;
  m[2][3] = 1 / d;
}

} // namespace Math
} // namespace Gcad

#endif

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

#ifndef _GCAD_MATRIXUTIL_H_
#define _GCAD_MATRIXUTIL_H_

#include "GcadAssertion.h"
#include "GcadMatrix.h"

namespace Gcad {
namespace Math {

/**
  @brief
*/        
struct MatrixUtil {

  /**
    @brief
      Zwrocenie informacji odpowiadajacej na pytanie:
      "Czy macierz nalezy do zbioru zawierajacego macierze diagonalne?"
    
    @remark
      Dla macierzy M, ktora jest diagonalna jej transpozycja jest rowna
      zrodlowej macierzy: transpose(M) == M
  */
  template<int MATRIX_DIMENSION, typename T>
  static
  bool
  isDiagonal(const Matrix<MATRIX_DIMENSION, MATRIX_DIMENSION, T>& m);
 
  template<int ROWS, int COLS, typename T>
  static 
  void
  add( const Matrix<ROWS, COLS, T>& lhs,
       const Matrix<ROWS, COLS, T>& rhs,
       Matrix<ROWS, COLS, T>* out );
   
  template<int ROWS, int COLS, typename T>
  static void
  sub( const Matrix<ROWS, COLS, T>& lhs,
       const Matrix<ROWS, COLS, T>& rhs,
       Matrix<ROWS, COLS, T>* out );

  template<int OUT_ROWS, int OUT_COLS, int IN, typename T>
  static void
  mul( const Matrix<OUT_ROWS, IN, T>& lhs,
       const Matrix<IN, OUT_COLS, T>& rhs,
       Matrix<OUT_ROWS, OUT_COLS, T>* out );

  template<int ROWS, int COLS, typename T>
  static 
  void
  mul( const Matrix<ROWS, COLS, T>& lhs,
       T scalar,
       Matrix<ROWS, COLS, T>* out );

  template<int ROWS, int COLS, typename T>
  static inline 
  void
  mul( T scalar,
       const Matrix<ROWS, COLS, T>& rhs,
       Matrix<ROWS, COLS, T>* out );
   
  template<int ROWS, int COLS, typename T>
  static 
  void
  transpose( const Matrix<ROWS, COLS, T>& matrix,
             Matrix<COLS, ROWS, T>* out );

  template<int ROW, int COL, int MATRIX_SIZE, typename T>
  static
  T
  cofactor(const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m);

  template<int MATRIX_SIZE, typename T>
  static
  T
  determinant(const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m);

  template<int MATRIX_SIZE, typename T>
  static
  bool
  inverse(const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m,
          Matrix<MATRIX_SIZE, MATRIX_SIZE, T>* out);

 private:
   // nie zaimplementowane
   MatrixUtil();
   MatrixUtil( const MatrixUtil& );
   MatrixUtil& operator =( const MatrixUtil& );

};


template<int MATRIX_DIMENSION, typename T>
bool
MatrixUtil
::isDiagonal(
  const Matrix<MATRIX_DIMENSION, MATRIX_DIMENSION, T>& m)
{
  for(int row = 0; row < MATRIX_DIMENSION; ++row)
    for(int col = 0; col < MATRIX_DIMENSION; ++col)
      if( (row != col && m[row][col] != static_cast<T>(0)) ||
        (row == col && m[row][col] == static_cast<T>(0)) )
      {
        return false;
      }
  return true;
}

template<int ROWS, int COLS, typename T> 
void 
MatrixUtil
::add( 
  const Matrix<ROWS, COLS, T>& lhs,
  const Matrix<ROWS, COLS, T>& rhs,
  Matrix<ROWS, COLS, T>* out ) 
{
  Utilities::assertion(out != 0,
    "Wskaznik macierzy nie ustawiony");

  for(int row = 0; row < ROWS; ++row)
    for(int col = 0; col < COLS; ++col)
      (*out)[row][col] = lhs[row][col] + rhs[row][col];
}

template<int ROWS, int COLS, typename T> 
void 
MatrixUtil
::sub( 
  const Matrix<ROWS, COLS, T>& lhs,
  const Matrix<ROWS, COLS, T>& rhs,
  Matrix<ROWS, COLS, T>* out ) 
{
  Utilities::assertion(out != 0,
    "Wskaznik macierzy nie ustawiony");

  for(int row = 0; row < ROWS; ++row)
    for(int col = 0; col < COLS; ++col)
      (*out)[row][col] = lhs[row][col] - rhs[row][col];
}
   

template<int OUT_ROWS, int OUT_COLS, int IN, typename T>
void
MatrixUtil
::mul( 
  const Matrix<OUT_ROWS, IN, T>& lhs,
  const Matrix<IN, OUT_COLS, T>& rhs,
  Matrix<OUT_ROWS, OUT_COLS, T>* out )
{
  Utilities::assertion(out != 0,
    "Wskaznik macierzy nie ustawiony");

  for(int col = 0; col < OUT_COLS; ++col)
    for(int row = 0; row < OUT_ROWS; ++row)
      for(int i = 0; i < IN; ++i)
        (*out)[row][col] += lhs[row][i] * rhs[i][col];
}

template<int ROWS, int COLS, typename T>
void 
MatrixUtil
::mul( 
  const Matrix<ROWS, COLS, T>& lhs,
  T scalar,
  Matrix<ROWS, COLS, T>* out )
{
  Utilities::assertion(out != 0,
    "Bledna wartosc wskaznika macierzy!");

  for(int row = 0; row < ROWS; ++row)
    for(int col = 0; col < COLS; ++col)
      (*out)[row][col] = lhs[row][col] * scalar;
}

template<int ROWS, int COLS, typename T> 
inline 
void 
MatrixUtil
::mul( 
  T scalar,
  const Matrix<ROWS, COLS, T>& rhs,
  Matrix<ROWS, COLS, T>* out ) 
{
  mul(rhs, scalar, out);
}

template<int ROWS, int COLS, typename T> 
void 
MatrixUtil
::transpose( 
  const Matrix<ROWS, COLS, T>& matrix,
  Matrix<COLS, ROWS, T>* out ) 
{
  Utilities::assertion(out != 0,
    "Wskaznik macierzy nie ustawiony!");

  for(int row = 0; row < ROWS; ++row)
    for(int col = 0; col < COLS; ++col)
      (*out)[col][row] = lhs[row][col];
}


template<int EXCLUDED_ROW, 
         int EXCLUDED_COL, 
         int MATRIX_SIZE, 
         typename T>
T
MatrixUtil
::cofactor(const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m)
{
  if(MATRIX_SIZE == 1) {
    return m[0][0];
  }
  else
  if(MATRIX_SIZE == 2) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
  }
  else 
  {  
    T detValue;

    for(int col = 0; col < MATRIX_SIZE; ++col) 
    {   
      Matrix<MATRIX_SIZE-1, MATRIX_SIZE-1, T> n;
      int fillCntCol = 0;
      int fillCntRow = 0;
      for(int c = 0; c < MATRIX_SIZE; ++c) {
        for(int r = 0; r < MATRIX_SIZE; ++r) {
          if(r == EXCLUDED_ROW)
            break;
          if(c != EXCLUDED_COL) {
            n[fillCntRow][fillCntCol] = m[r][c];
            if(++fillCntCol == MATRIX_SIZE)
              ++fillCntRow;
          }
        }
      }

      bool MINUS = static_cast<bool>((EXCLUDED_ROW + EXCLUDED_COL) % 2);
      const int ROW_TO_EXCLUDE = (EXCLUDED_ROW - 1 < 0)? 
        EXCLUDED_ROW + 1 : 
        EXCLUDED_ROW - 1;
      const int COL_TO_EXCLUDE = (EXCLUDED_COL - 1 < 0)? 
        EXCLUDED_COL + 1 : 
        EXCLUDED_COL - 1;

      detValue = (MINUS? 
        -m[EXCLUDED_ROW][EXCLUDED_COL] : 
        m[EXCLUDED_ROW][EXCLUDED_COL]) *
        cofactor<ROW_TO_EXCLUDE, COL_TO_EXCLUDE>(n);
    }
    
    return detValue;
  }  
}

template<int MATRIX_SIZE, typename T>
T
MatrixUtil
::determinant(
  const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m)
{
  Utilities::assertion(MATRIX_SIZE <= 0,
    "Wyznacznik mozna obliczyc dla macierzy rzedu dodatniego!");
  return cofactor<0,0>(m);
  /*
  if(MATRIX_SIZE == 1) {
    return m[0][0];
  }
  else
  if(MATRIX_SIZE == 2) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
  }
  else 
  {  
    T detValue;

    for(int col = 0; col < MATRIX_SIZE; ++col) 
    {   
      Matrix<MATRIX_SIZE-1, MATRIX_SIZE-1, T> n;
      int fillCntCol = 0;
      for(int c = 0; c < MATRIX_SIZE; ++c) {
        for(int r = 1; r < MATRIX_SIZE; ++r) {
          if(c != col) {
            n[r-1][fillCntCol] = m[r][c];
            ++fillCntCol;
          }
        }
      }

      bool POSITIVE_SIGN = static_cast<bool>( !(col % 2) );

      detValue = (POSITIVE_SIGN? m[0][col] : -m[0][col]) *
        determinant(n);
    }
    
    return detValue;
  } */ 
}

template<int MATRIX_SIZE, typename T>
bool
MatrixUtil
::inverse(
  const Matrix<MATRIX_SIZE, MATRIX_SIZE, T>& m,
  Matrix<MATRIX_SIZE, MATRIX_SIZE, T>* out)
{
  Utilities::assertion(out != 0,
    "Wskaznik macierzy nie ustawiony!");

  T det = determinant(m);
  if(det == 0)
    return false;

  Matrix<MATRIX_SIZE, MATRIX_SIZE, T> cofactorMatrix;
  for(int r = 0; r < MATRIX_SIZE; ++r)
    for(int c = 0; c < MATRIX_SIZE; ++c)
      cofactorMatrix[r][c] = cofactor<r,c>(m);
  
  mul(1 / det, cofactorMatrix, out);

  return true;
}

/* 

//-------------------------------------------------------------------------------
// IMPLEMENTACJA WZORCA KLASY CMatrixNxN< ROWS, COLS >

//-------------------------------------------------------------------------------
//  konstruktor wykorzystany przy wyliczaniu wyznacznika, do stworzenia 
//  macierzy, przez odciecie z bierzacej macierzy pierwszego wiersza, orac
//  colErase kolumny. jak latwo sie mozna domyslec, macierz ta bedzie o jeden
//  rzad mniejsza od macierzy z ktorej powstaje ( argument konstruktora ), stad
//  w nawiasach katowych +1 osobno dla kolumn i wierszy
//-------------------------------------------------------------------------------

template< int ROWS, int COLS >
CMatrixNxN< ROWS, COLS >::CMatrixNxN
( const CMatrixNxN< ROWS + 1, COLS + 1 >& m, int colErase ) {
  for( int r = 0; r < ROWS; ++r )
    for( int c = 0, i = 0; c < COLS + 1; ++c ) {
      if( c == colErase )
        continue;
      _m[ r ][ i ] = m._m[ r + 1 ][ c ];
      ++i;
    }
}

//-------------------------------------------------------------------------------
//  wyznaczenie macierzy odwrotnej ( przy zalozeniu det( A ) != 0 ) do danej 
//  macierzy rzedu drugiego przedstawia sie wzorem:
//
//     | a  b |                             | d -b |
//  A( |      | )      A-1 = 1 / det( A ) * |      |
//     | c  d |                             |-c  a |
//
//
//-------------------------------------------------------------------------------
/*
inline bool InvertCMatrixNxN
( const CMatrixNxN< 2, 2 >& m, CMatrixNxN< 2, 2 >* out ) {
  REAL det = DetCMatrixNxN( m );
  if( !det )
    return false;
  out->_m[ 0 ][ 0 ] = m._m[ 1 ][ 1 ];
  out->_m[ 0 ][ 1 ] = -m._m[ 0 ][ 1 ];
  out->_m[ 1 ][ 0 ] = -m._m[ 1 ][ 0 ];
  out->_m[ 1 ][ 1 ] = m._m[ 0 ][ 0 ];
  *out *= ( 1.0f / det );
  return true;
}
*/
/*

//-------------------------------------------------------------------------------

export template< int ROWS, int COLS >
CMatrixNxN< ROWS, COLS >& CMatrixNxN< ROWS, COLS >::operator +=
( const CMatrixNxN< ROWS, COLS >& m ) {
  for( int r = 0; r < ROWS; ++r )
    for( int c = 0; c < COLS; ++c )
      _m[ r ][ c ] += m._m[ r ][ c ];
  return *this;
}

//-------------------------------------------------------------------------------

export template< int ROWS, int COLS >
CMatrixNxN< ROWS, COLS >& CMatrixNxN< ROWS, COLS >::operator -=
( const CMatrixNxN< ROWS, COLS >& m ) {
  for( int r = 0; r < ROWS; ++r )
    for( int c = 0; c < COLS; ++c )
      _m[ r ][ c ] -= m._m[ r ][ c ];
  return *this;
}

//-------------------------------------------------------------------------------

export template< int ROWS, int COLS >
CMatrixNxN< ROWS, COLS >& CMatrixNxN< ROWS, COLS >::operator *= ( float value ) {
  for( int r = 0; r < ROWS; ++r )
    for( int c = 0; c < COLS; ++c )
      _m[ r ][ c ] *= value;
   return *this;
}


export template< int Y, int X > CMatrixNxN< Y, X > operator /
( const CMatrixNxN< Y, X >& m, float value ) {
  CMatrixNxN< Y, X > temp = m;
  float val = 1 / value;
  return m * value;
}

//-------------------------------------------------------------------------------

export template< int I >
float DetCMatrixNxN( const CMatrixNxN< I, I >& m ) {
  switch( I ) {
  case 1:
    return m._m[ 0 ][ 0 ];
  case 2:
    return m._m[ 0 ][ 0 ] * m._m[ 1 ][ 1 ] - m._m[ 0 ][ 1 ] * m._m[ 1 ][ 0 ];
  default:
    {
      float sum = 0.0f;
      for( int c = 0; c < I; ++c ) {
        CMatrixNxN< I - 1, I - 1 > temp( m, c );
        sum += pow( -1, c + 2 ) * m._m[ 0 ][ c ] * DetCMatrixNxN( temp );
      }
      return sum;
    }
  }
}

//-------------------------------------------------------------------------------


*/

} // namespace Math
} // namespace Gcad

#endif

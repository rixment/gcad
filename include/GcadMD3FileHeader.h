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

#ifndef _GCAD_MD3FILEHEADER_H_
#define _GCAD_MD3FILEHEADER_H_

#include "GcadException.h"
#include <iostream>
#include <string>

namespace Gcad {
namespace Framework {

/**
  @brief
    Struktura organizacyjna wartosci naglowka pliku formatu MD3 - Quake 3
*/
class MD3FileHeader {
 public:
   /**
     @brief
       Wyjatek zglaszany w czasie wystapienia bledow na etapie
       wczytywania naglowka pliku formatu MD3
   */
   class BadHeaderData : public Gcad::Utilities::Exception {
    public:
      /**
        @brief
          Konstruktor obiektu wyjatku
      */
      BadHeaderData( std::istream&  badStream,
                     const char*    exceptionDecription )
        : Gcad::Utilities::Exception( exceptionDecription )
        , badStream_( badStream )
      {}
      
      /**
        @brief
          Akcesor do strumienia, ktorego niepoprawny stan byl powodem
          powstania sytuacji wyjatkowej
      */
      std::istream& badStream() { return badStream_; }
    
    private:
      std::istream& badStream_;
   };

   /**
     @brief
       Konstruktor pobierajacy strumien danych formatu MD3
     
     @exception
       MD3FileHeader::BadHeaderData
   */   
   MD3FileHeader( std::istream& inputHeaderData );

   std::string  id()          const;
   std::string  fileName()    const;
   int          version()     const;
   int          framesNum()   const;     
   int          tagsNum()     const;
   int          meshesNum()   const;
   int          maxSkinsNum() const;
   int          headerSize()  const;
   int          tagStart()    const;
   int          meshStart()   const;
   int          fileSize()    const;

 private:
   char  id_[ 4 ];        /**< @brief Magiczna wartosc, zawsze rowna "IDP3" */
   int   version_;        /**< @brief Numer wersji, zawsze rowny 15 */
   char  fileName_[ 68 ]; /**< @brief Nazwa pliku */
   int   framesNum_;      /**< @brief Ilosc klatek kluczowych modelu */
   int   tagsNum_;        /**< @brief Ilosc lacznikow siatek jednej klatki */
   int   meshesNum_;      /**< @brief Ilosc siatek klatki */
   int   maxSkinsNum_;    /**< @brief Maksymalna ilosc uzywanych tekstur */
   int   headerSize_;     /**< @brief Rozmiar naglowka pliku */
   int   tagStart_;       /**< @brief Pozycja poczatku opisu lacznikow klatek */
   int   meshStart_;      /**< @brief Poczatek pozycji struktury siatki modelu */
   int   fileSize_ ;      /**< @brief Rozmiar pliku formatu MD3 */
};

//
inline std::string MD3FileHeader
::id() const 
{
  return id_;
}

//
inline std::string MD3FileHeader
::fileName() const 
{
  return fileName_;
}

//
inline int MD3FileHeader
::version() const 
{
  return version_;
}

//
inline int MD3FileHeader
::framesNum() const
{
  return framesNum_;
}

//
inline int MD3FileHeader
::tagsNum() const
{
  return tagsNum_;
}

//
inline int MD3FileHeader
::meshesNum() const
{
  return meshesNum_;
}

//
inline int MD3FileHeader
::maxSkinsNum() const
{
  return maxSkinsNum_;
}

//
inline int MD3FileHeader
::headerSize() const
{
  return headerSize_;
}

//
inline int MD3FileHeader
::tagStart() const
{
  return tagStart_;
}

//
inline int MD3FileHeader
::meshStart() const
{
  return meshStart_;
}

//
inline int MD3FileHeader
::fileSize() const
{
  return fileSize_;
}

} // namespace Framework
} // namespace Gcad

#endif

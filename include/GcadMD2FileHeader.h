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

#ifndef _GCAD_MD2FILEHEADER_H_
#define _GCAD_MD2FILEHEADER_H_

#include "GcadException.h"
#include <iostream>

namespace Gcad {
namespace Framework {

/** 
  @brief
    Klasa reprezentujaca wartosci naglowka pliku formatu MD2 - Quake2
*/
class GCAD_EXPORT MD2FileHeader {
 public:
   /** 
     @brief
       Klasa obiektu wyjatku powstalego w sytuacji, gdy dane odczytane
       z naglowka pliku wskazuja na niepoprawny format 
   */
   class GCAD_EXPORT BadHeaderData : public Utilities::Exception {
     friend class MD2FileHeader;
     BadHeaderData( const char* errorDescription )
       : Utilities::Exception( errorDescription )
     {}
   };

   /** 
     @brief
       Klasa obiektu wyjatku, ktorej instancja jest zglaszana w sytuacji,
       gdy podczas odczytu danych zostanie ujawniony blad strumienia wejscia
   */
   class GCAD_EXPORT InputStreamInBadState : public Utilities::Exception {
     friend class MD2FileHeader;
     InputStreamInBadState( const char*    errorDescription, 
                            std::istream&  badInputStream )
       : Utilities::Exception( errorDescription )
       , badInputStream_( badInputStream )
     {}
      
    public:
      /**
        @brief
          Akcesor strumienia znajdujacego sie w blednym stanie
      */
      std::istream& badInputStream() { return badInputStream_; }
    
    private:
      std::istream& badInputStream_; /**< @brief Bledny strumien */
   };

   /** 
     @brief 
       Konstruktor pobierajacy strumien wejsciowy, 
       sprzezony ze zrodlem danych naglowka pliku formatu MD2 

     @exception
       BadHeaderData Instancja rzucana w przypadku odczytania zawartosci
       naglowka pliku i stwierdzeniu, iz wartosci go reprezentujace
       sa niezgodne z oczekiwanymi

     @exception
       InputStreamInBadState Obiekt wyjatku zglaszany w sytuacji
       odczytu naglowka ze strumienia, przy czym rezultatem zakonczenia 
       pobierania danych jest wprowadzenie strumienia w stan bledu.
       Jest to jednoznaczna informacja, iz naglowek pliku nie zostal
       odczytany, z powodu niezgodnosci danych w nim zawartych
   */
   MD2FileHeader( std::istream& inputMD2HeaderData );
 
   // Akcesory do wartosci naglowka
   int identifier()       const;
   int version()          const;
   int skinWidth()        const;
   int skinHeight()       const;
   int frameSize()        const;
   int numSkins()         const;
   int numVerts()         const;
   int numTexCoords()     const;
   int numPolys()         const;
   int numOpenGLCmds()    const;
   int numFrames()        const;
   int offsetSkins()      const;
   int offsetTexCoords()  const;
   int offsetPolys()      const;
   int offsetFrames()     const;
   int offsetOpenGLCmds() const;
   int offsetEnd()        const;

 private:
   int  identifier_;    /**< Magiczna liczba zawsze rowna IDP2 */
   int  version_;       /**< Magiczna liczba zawsze rowna 8 */
   int  skinWidth_;     /**< Szerokosc skory modelu - szerokosc tekstury */
   int  skinHeight_;    /**< Wysokosc skory modelu - wysokosc tekstury */
   int  frameSize_;     /**< Ilosc bajtow odpowiadajaca jednej klatke animacji */
   int  numSkins_;      /**< Ilosc skor */
   int  numVerts_;      /**< Ilosc wierzcholkow jednej klatki animacji */
   int  numTexCoords_;  /**< Ilosc wspolrzednych tekstury */
   int  numPolys_;      /**< Ilosc poligonow tworzacych model */
   int  numOpenGLCmds_; /**< Instrukcje wspomagajace optymalizacje OpenGL */
   int  numFrames_;     /**< Ilosc klatek animacji (standardowa wartosc 198) */
  
   // Kolejno wyrazone w bajtach przesuniecia 
   // danych od poczatku pliku
   int  offsetSkins_;
   int  offsetTexCoords_;
   int  offsetPolys_;
   int  offsetFrames_;
   int  offsetOpenGLCmds_;
   int  offsetEnd_;
};


//
inline int MD2FileHeader
::identifier() const
{
  return identifier_;
}

//
inline int MD2FileHeader
::version() const
{
  return version_;
}

//
inline int MD2FileHeader
::skinWidth() const
{
  return skinWidth_;
}

//
inline int MD2FileHeader
::skinHeight() const
{
  return skinHeight_;
}

//
inline int MD2FileHeader
::frameSize() const
{
  return frameSize_;
}

//
inline int MD2FileHeader
::numSkins() const
{
  return numSkins_;
}

//
inline int  MD2FileHeader
::numVerts() const
{
  return numVerts_;
}

//
inline int MD2FileHeader
::numTexCoords() const
{
  return numTexCoords_;
}

//
inline int MD2FileHeader
::numPolys() const
{
  return numPolys_;
}

//
inline int MD2FileHeader
::numOpenGLCmds() const 
{
  return numOpenGLCmds_;
}

//
inline int MD2FileHeader
::numFrames() const
{
  return numFrames_;
}
  
//
inline int MD2FileHeader
::offsetSkins() const
{
  return offsetSkins_;
}

//
inline int MD2FileHeader
::offsetTexCoords() const
{
  return offsetTexCoords_;
}

//
inline int MD2FileHeader
::offsetPolys() const
{
  return offsetPolys_;
}

//
inline int MD2FileHeader
::offsetFrames() const
{
  return offsetFrames_;
}

//
inline int MD2FileHeader
::offsetOpenGLCmds() const
{
  return offsetOpenGLCmds_;
}

//
inline int MD2FileHeader
::offsetEnd() const
{
  return offsetEnd_;
}

} // namespace Framework
} // namespace Gcad

#endif

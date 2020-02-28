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

#ifndef _GCAD_MD3MESHHEADER_H_
#define _GCAD_MD3MESHHEADER_H_

#include <iostream>
#include <string>

namespace Gcad {
namespace Framework {

/**
  @brief
    Opis naglowka komponentu, z ktorych utworzony jest model
*/
class MD3MeshHeader {
 public:
   /**
     @brief
       Konstruktor wczytujacy dane z wejscia
   */
   MD3MeshHeader( std::istream& meshHeaderStream );

   std::string  id()             const;
   std::string  name()           const;
   int          framesNum()      const;
   int          skinNum()        const;
   int          verticesNum()    const;
   int          trianglesNum()   const;
   int          trianglesStart() const;
   int          headerSize()     const;
   int          uvStart()        const;
   int          vertexStart()    const;
   int          meshSize()       const;

 private:
   char  id_[ 4 ];        /**< @brief Identyfikator "ID" */
   char  name_[ 68 ];
   int   framesNum_;      /**< @brief Ilosc klatek siatki */
   int   skinNum_;        /**< @brief Ilosc tekstur siatki */
   int   verticesNum_;    /**< @brief Ilosc wierzcholkow siatki */
   int   trianglesNum_;   /**< @brief Ilosc trojkatow */
   int   trianglesStart_; /**< @brief Pozycja poczatkowa danych trojkatow */
   int   headerSize_;     /**< @brief Rozmiar naglowka siatki */
   int   uvStart_;        /**< @brief Pozycja poczatkowa wektora tekstur */
   int   vertexStart_;    /**< @brief Pozycja poczatkowa indeksow wierzcholkow */
   int   meshSize_;       /**< @brief Rozmiar struktury siatki */
};

//
inline MD3MeshHeader
::MD3MeshHeader( std::istream& meshHeaderStream )
{
  meshHeaderStream.read(
    reinterpret_cast< char* >( this ),
    sizeof( MD3MeshHeader ) );
}

//
inline std::string MD3MeshHeader
::id() const
{
  return id_;
}

//
inline std::string MD3MeshHeader
::name() const
{
  return name_;
}

//
inline int MD3MeshHeader
::framesNum() const
{
  return framesNum_;
}

//
inline int MD3MeshHeader
::skinNum() const
{
  return skinNum_;
}

//
inline int MD3MeshHeader
::verticesNum() const
{
  return verticesNum_;
}

//
inline int MD3MeshHeader
::trianglesNum() const
{
  return trianglesNum_;
}

//
inline int MD3MeshHeader
::trianglesStart() const
{
  return trianglesStart_;
}

//
inline int MD3MeshHeader
::headerSize() const
{
  return headerSize_;
}

//
inline int MD3MeshHeader
::uvStart() const
{
  return uvStart_;
}

//
inline int MD3MeshHeader
::vertexStart() const
{
  return vertexStart_;
}

//
inline int MD3MeshHeader
::meshSize() const
{
  return meshSize_;
}

} // namespace Framework
} // namespace Gcad

#endif

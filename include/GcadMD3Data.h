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

#ifndef _GCAD_MD3DATA_H_
#define _GCAD_MD3DATA_H_

#include "GcadMD3FileHeader.h"
#include "GcadMD3BoneFrame.h"
#include "GcadMD3Tag.h"
#include "GcadMD3Mesh.h"
#include "GcadRefCountPtr.h"
#include <string>
#include <vector>

namespace Gcad {
namespace Framework {

/**
  @brief
    Klasa wykonujaca dzialanie odczytania danych formatu MD3 - Quake3    
*/
class MD3Data {
 public:
   typedef Gcad::Utilities::RefCountPtr<MD3FileHeader>  SmartPtrMD3FileHeader;
   
   typedef Gcad::Utilities::RefCountPtr< MD3BoneFrame >  SmartPtrMeshesBoneFrame;
   typedef Gcad::Utilities::RefCountPtr< MD3Tag >        SmartPtrMeshesTag;
   typedef Gcad::Utilities::RefCountPtr< MD3Mesh >       SmartPtrMesh;
   
   typedef std::vector< SmartPtrMeshesBoneFrame >  MeshesBoneFrames;
   typedef std::vector< SmartPtrMeshesTag >        MeshesTags;
   typedef std::vector< SmartPtrMesh >             Meshes;

   /** 
     @brief 
       Klasa uzywana do sprzezenia informacji o jednej siatce modelu

     @see 
       meshInfo( size_t meshIndex )
   */
   class MeshInfo {
    public:
      MeshInfo( const SmartPtrMeshesBoneFrame&  frame,
                const SmartPtrMeshesTag&        tag,
                const SmartPtrMesh&             mesh )
        : frame_( frame ),
          tag_( tag ),
          mesh_( mesh )
      {}

      MD3BoneFrame&  frame() { return *frame_; }
      MD3Tag&        tag()   { return *tag_; }
      MD3Mesh&       mesh()  { return *mesh_; }
    
    private:
      SmartPtrMeshesBoneFrame  frame_;
      SmartPtrMeshesTag        tag_;
      SmartPtrMesh             mesh_;
   };

   /**
     @brief
       Konstruktor pobierajacy nazwe pliku z danymi formatu MD3
   */
   MD3Data( const std::string& fileName );

   /**
     @brief
       Udostepnienie informacji dotyczacej naglowka pliku
   */
   MD3FileHeader fileHeader() const;

   /**
     @brief
       Pobranie informacji dotyczacej zadanej siatki modelu

     @param
       meshIndex Wartosc indeksu komponentu modelu. Metoda spowoduje
       blad asercji w sytuacji otrzymania argumentu spoza dozwolonego
       zakresu < 1, meshCount() )
   */
   MeshInfo meshInfo( size_t meshIndex );

   /**
     @brief
       Informacja na temat ilosci siatek, z ktorych sklada sie model
   */
   size_t meshesCount() const;
 
 private:
   void readBoneFrames( std::istream& boneFrameData );
   void readTags      ( std::istream& tagsData );
   void readMeshes    ( std::istream& meshesData );
 
 private:
   SmartPtrMD3FileHeader  fileHeader_;

   MeshesBoneFrames  boneFrames_;
   MeshesTags        tags_;
   Meshes            meshes_;
};


//
inline MD3FileHeader MD3Data
::fileHeader() const
{
  return *fileHeader_;
}

//
inline size_t MD3Data
::meshesCount() const
{
  return meshes_.size();
}

//
inline MD3Data::MeshInfo MD3Data
::meshInfo( size_t meshIndex )
{
  Gcad::Utilities::assertion( 
    meshIndex >= 0 &&
    meshIndex < meshes_.size(),
    "MD3Data::meshInfo(size_t): Indeks spoza zakresu!" );

  return MeshInfo( 
    boneFrames_[ meshIndex ],
    tags_[ meshIndex ],
    meshes_[ meshIndex ] );
}

} // namespace Framework
} // namespace Gcad

#endif

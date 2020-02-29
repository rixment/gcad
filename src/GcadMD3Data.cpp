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

#include "GcadMD3Data.h"
#include <fstream>

namespace Gcad {
namespace Framework {

//
MD3Data
::MD3Data( const std::string& fileName )
{
  std::ifstream  inputData( fileName.c_str(),
                            std::ios_base::binary );
  
  fileHeader_ = new MD3FileHeader( inputData );

  readBoneFrames( inputData );
  readTags( inputData );
  readMeshes( inputData );
}

//
void MD3Data
::readBoneFrames( std::istream& boneFrameData )
{
  boneFrames_.resize( fileHeader_->framesNum() );
 
  for( int bonesCount = 0; 
    bonesCount < fileHeader_->framesNum(); 
    ++bonesCount )
  {
    boneFrames_[ bonesCount ] = new MD3BoneFrame( boneFrameData );
  }
}

//
void MD3Data
::readTags( std::istream& tagsData )
{
  const int TAGS_COUNT = fileHeader_->framesNum() * fileHeader_->tagsNum();
  
  tags_.resize( TAGS_COUNT );

  for( int tagCount = 0; 
    tagCount < TAGS_COUNT;
    ++tagCount )
  {
    tags_[ tagCount ] = new MD3Tag( tagsData );
  }
}

//
void MD3Data
::readMeshes( std::istream& meshesData )
{
  meshes_.resize( fileHeader_->meshesNum() );
  
  for( int meshesCount = 0; 
    meshesCount < fileHeader_->meshesNum(); 
    ++meshesCount )
  {
    std::istream::pos_type meshOffset = meshesData.tellg();
    
    meshes_[ meshesCount ] = new MD3Mesh( meshesData );

    meshesData.seekg( meshOffset );
    meshesData.seekg( 
      meshes_[ meshesCount ]->meshHeader().meshSize(), 
      std::ios_base::cur );
  }
}


} // namespace Framework
} // namespace Gcad

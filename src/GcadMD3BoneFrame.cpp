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

#include "GcadMD3BoneFrame.h"

namespace Gcad {
namespace Framework {

//
MD3BoneFrame
::MD3BoneFrame( std::istream& boneFrameData )
{
  // Oryginalna reprezentacja danych skladowych komponentu modelu,
  // odpowiadajaca formatowi danych wejsciowych - MD3
  struct {
    float minBox[ 3 ];
    float maxBox[ 3 ];
    float origin[ 3 ];
    float radius;
  } readedFrameData;

  boneFrameData.read( 
    reinterpret_cast< char* >( &readedFrameData ), 
    sizeof( readedFrameData ) );

  minBBox_.setX( readedFrameData.minBox[ 0 ] );
  minBBox_.setY( readedFrameData.minBox[ 1 ] );
  minBBox_.setZ( readedFrameData.minBox[ 2 ] );

  maxBBox_.setX( readedFrameData.maxBox[ 0 ] );
  maxBBox_.setY( readedFrameData.maxBox[ 1 ] );
  maxBBox_.setZ( readedFrameData.maxBox[ 2 ] );

  originPos_.setX( readedFrameData.origin[ 0 ] );
  originPos_.setY( readedFrameData.origin[ 1 ] );
  originPos_.setZ( readedFrameData.origin[ 2 ] );

  sphereRadius_ = readedFrameData.radius;
   
  boneFrameData.read( name_, sizeof( name_ ) ); 
}

} // namespace Framework
} // namespace Gcad

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

#include "GcadMD3Tag.h"

namespace Gcad {
namespace Framework {

MD3Tag
::MD3Tag( std::istream& tagDataStream )
{
  tagDataStream.read( name_, sizeof( name_ ) );

  // Oryginalna struktura organizacyjna danych pobieranych z wejscia
  struct {
    float pos[ 3 ];
    float orient[ 3 ][ 3 ];
  } tagData;
  
  tagDataStream.read(
    reinterpret_cast< char* >( &tagData ),
    sizeof( tagData ) );

  position_.setX( tagData.pos[ 0 ] );
  position_.setY( tagData.pos[ 1 ] );
  position_.setZ( tagData.pos[ 2 ] );

  for( int row = 0; row < 3; ++row )
    for( int col = 0; col < 3; ++col )
      orientation_[ row ][ col ] = tagData.orient[ row ][ col ];

  matrixToQuaternion(orientation_, &quatOrient_);
}

} // namespace Framework
} // namespace Gcad

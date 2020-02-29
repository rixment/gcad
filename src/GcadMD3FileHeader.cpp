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

#include "GcadMD3FileHeader.h"
#include <string>

namespace Gcad {
namespace Framework {

MD3FileHeader
::MD3FileHeader( std::istream& inputHeaderData )
{
  const int moveToBeginStream = 0;
  inputHeaderData.seekg( moveToBeginStream, std::ios_base::beg );
  inputHeaderData.read(
    reinterpret_cast< char* >( this ),
    sizeof( MD3FileHeader ) );

  const std::string  idFileHeaderChecker( id_, sizeof( id_ ) );
  const int          validVersion = 15;

  if( !inputHeaderData || 
    idFileHeaderChecker != std::string( "IDP3" ) ||
    version_ != validVersion ) 
  {
    throw BadHeaderData( 
      inputHeaderData, 
      "MD3FileHeader::MD3FileHeader(std::istream&): Nieprawidlowy format pliku!" );
  }
}

} // namespace Framework
} // namespace Gcad

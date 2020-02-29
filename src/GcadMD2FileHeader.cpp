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

#include "GcadMD2FileHeader.h"

namespace Gcad {
namespace Framework {

MD2FileHeader
::MD2FileHeader( std::istream& inputMD2HeaderData ) 
{
  if( !inputMD2HeaderData ) {
    throw InputStreamInBadState(
      "MD2FileHeader::MD2FileHeader(std::istream&): Strumien w stanie bledu!",
      inputMD2HeaderData );
  }

  const int MOVE_TO_BEGIN = 0;
  inputMD2HeaderData.seekg( MOVE_TO_BEGIN, std::ios_base::beg );
  inputMD2HeaderData.read( 
    reinterpret_cast< char* >( this ),
    sizeof( MD2FileHeader ) );

  const int MD2_VERSION   = 8;
  const int MD2_ID_FORMAT = 'I' | ( 'D' << 8 ) | ( 'P' << 16 ) | ( '2' << 24 );

  if( !inputMD2HeaderData ) {
    throw InputStreamInBadState(
      "MD2FileHeader::MD2FileHeader(std::istream&): Strumien w stanie bledu!",
      inputMD2HeaderData );
  } 
  else if( identifier_ != MD2_ID_FORMAT ||
    version_ != MD2_VERSION ) 
  {
    throw BadHeaderData( "MD2FileHeader::MD2FileHeader(std::istream&): "
      "Nieprawidlowy format pliku!" );
  }
}

} // namespace Framework
} // namespace Gcad

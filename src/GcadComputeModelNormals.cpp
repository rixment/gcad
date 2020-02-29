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

#include "GcadComputeModelNormals.h"

namespace Gcad {
namespace Framework {

//
ComputeModelNormals::NormalsFrameSmartPtr ComputeModelNormals
::computeAproximatedNormals( PolygonsNormalsSmartPtr polygonsNormals )
{
  NormalsFrameSmartPtr  aproximatedVerticesNormals = 
    new NormalsFrame( polyIndicesForVertices_.size() );

  NormalsFrame&  normals = *aproximatedVerticesNormals;
  
  for( size_t  vertexIndex = 0;
    vertexIndex < polyIndicesForVertices_.size();
    ++vertexIndex )
  {
    IndicesToPolyArray&  vertexPolyIndices = 
      polyIndicesForVertices_[ vertexIndex ];
    
    const size_t  POLY_INDICES_COUNT = vertexPolyIndices.size();
  
    for( IndicesToPolyArrayItor  polyIndicesItor = vertexPolyIndices.begin();
      polyIndicesItor != vertexPolyIndices.end();
      ++polyIndicesItor )
    {
		  normals[ vertexIndex ] += ( *polygonsNormals )[ *polyIndicesItor ];
    } 

    normals[ vertexIndex ] /= static_cast< float >( POLY_INDICES_COUNT );
  }

  return aproximatedVerticesNormals;
}

} // namespace Framework
} // namespace Gcad

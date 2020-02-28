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

#ifndef _GCAD_SH2DATAMODEL_H_
#define _GCAD_SH2DATAMODEL_H_

#include "GcadBase.h"
#include "GcadRefCountPtr.h"
#include "GcadVector2.h"
#include "GcadVector3.h"
#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace Gcad {
namespace Framework {

class GCAD_EXPORT Sh2DataModel {
 public:
   typedef Math::Vector2<float> Vec2;
   typedef Math::Vector3<float> Vec3;
 
   typedef std::vector<Vec3>                     FrameVertices;
   typedef FrameVertices::const_iterator         FrameVerticesConstItor;
   typedef std::pair<FrameVerticesConstItor,
                     FrameVerticesConstItor>     BeginEndItorVerticesFrame;
   typedef Utilities::RefCountPtr<FrameVertices> FrameVerticesRefPtr;
   typedef std::vector<FrameVerticesRefPtr>      VerticesKeyFrames;
   
   typedef std::vector<Vec3>                    FrameNormals;
   typedef FrameNormals::const_iterator         FrameNormalsConstItor;
   typedef std::pair<FrameNormalsConstItor,
                     FrameNormalsConstItor>     BeginEndItorNormalsFrame;
   typedef Utilities::RefCountPtr<FrameNormals> FrameNormalsRefPtr;
   typedef std::vector<FrameNormalsRefPtr>      NormalsKeyFrames;

   typedef std::vector<Vec2>                      TextureCoordinates;
   typedef TextureCoordinates::const_iterator     TextureCoordinatesConstItor;
   typedef std::pair<TextureCoordinatesConstItor,
                     TextureCoordinatesConstItor> BeginEndItorTextureCoordinates;

 public:
   Sh2DataModel(const std::string& fileName);
   Sh2DataModel(std::istream& sh2Data);

   BeginEndItorVerticesFrame getKeyFrameVertices(int frameNum) const;
   BeginEndItorNormalsFrame getKeyFrameNormals(int frameNum) const;
   BeginEndItorTextureCoordinates getTextureCoordinates() const;

   int framesCount() const;
   int verticesPerFrameCount() const;
   int polygonsCount() const;
   int texturesCoordinatesCount() const;

 private:
   VerticesKeyFrames  verticesFrames_;
   NormalsKeyFrames   normalsFrames_;
   TextureCoordinates texturesCoordinates_;

 private:
   int framesCount_;
   int verticesPerFrameCount_;
   int polygonsCount_;
   int texturesCoordinatesCount_;

 private:
   void loadDataFromStream(std::istream& sh2input);
};

} // namespace Framework
} // namespace Gcad

#endif

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

#include "GcadSh2DataModel.h"
#include "GcadStdIos.h"
#include "GcadMD2Data.h"
#include "GcadAssertion.h"

namespace Gcad {
namespace Framework {

using namespace Utilities;

int Sh2DataModel
::framesCount() const 
{
  return framesCount_; 
}

int Sh2DataModel
::verticesPerFrameCount() const 
{ 
  return verticesPerFrameCount_; 
}

int Sh2DataModel
::polygonsCount() const
{
  return polygonsCount_;
}
 
int Sh2DataModel
::texturesCoordinatesCount() const
{
  return texturesCoordinatesCount_;
}

Sh2DataModel
::Sh2DataModel(const std::string& fileName)
{
  std::ifstream inputSh2(fileName.c_str(), std::ios_base::binary);
  loadDataFromStream(inputSh2);
}

Sh2DataModel
::Sh2DataModel(std::istream& sh2Data)
{
  loadDataFromStream(sh2Data);
}

void Sh2DataModel
::loadDataFromStream(std::istream& sh2input)
{
  readFromStdStream( sh2input, &framesCount_, 1 );
  readFromStdStream( sh2input, &verticesPerFrameCount_, 1 );
  readFromStdStream( sh2input, &polygonsCount_, 1 );
  readFromStdStream( sh2input, &texturesCoordinatesCount_, 1 );

  verticesFrames_.resize(framesCount());
  normalsFrames_.resize(framesCount());
  texturesCoordinates_.resize(texturesCoordinatesCount());

  for(int frameIndex = 0;
    frameIndex < framesCount();
    ++frameIndex)
  {
    FrameVerticesRefPtr frame( new FrameVertices(verticesPerFrameCount()) );
    readFromStdStream( sh2input, &frame->front(), verticesPerFrameCount() );
    verticesFrames_[frameIndex] = frame;
  }

  for(int frameIndex = 0;
    frameIndex < framesCount();
    ++frameIndex)
  {
    FrameVerticesRefPtr frame( new FrameNormals(verticesPerFrameCount()) );
    readFromStdStream( sh2input, &frame->front(), verticesPerFrameCount() );
    normalsFrames_[frameIndex] = frame;
  }

  readFromStdStream( 
    sh2input, 
    &texturesCoordinates_.front(),
    texturesCoordinatesCount() );
}

Sh2DataModel::BeginEndItorVerticesFrame 
Sh2DataModel
::getKeyFrameVertices(int frameNum) const
{
  Utilities::assertion(frameNum >= 0 && frameNum < framesCount(),
    "Wartosc okreslajaca klatke kluczowa spoza dozwolonego zakresu!");

  return std::make_pair(
    verticesFrames_[frameNum]->begin(), 
    verticesFrames_[frameNum]->end());
}

Sh2DataModel::BeginEndItorNormalsFrame
Sh2DataModel
::getKeyFrameNormals(int frameNum) const
{
  Utilities::assertion(frameNum >= 0 && frameNum < framesCount(),
    "Wartosc okreslajaca klatke kluczowa spoza dozwolonego zakresu!");

  return std::make_pair(
    normalsFrames_[frameNum]->begin(), 
    normalsFrames_[frameNum]->end());
}

Sh2DataModel::BeginEndItorTextureCoordinates
Sh2DataModel
::getTextureCoordinates() const
{
  return std::make_pair(
    texturesCoordinates_.begin(), 
    texturesCoordinates_.end());
}

} // namespace Framework
} // namespace Gcad

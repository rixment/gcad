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

#include "GcadMS3DModel.h"
#include "GcadRefArrayCntPtr.h"
#include "GcadStdIos.h"
#include <algorithm>
#include <string>

using namespace std;

namespace {

using namespace Gcad::Utilities;
using namespace Gcad::Framework;

/* 
typedef __declspec(align(1)) char            CHAR;
typedef __declspec(align(1)) unsigned char   UCHAR;
typedef __declspec(align(1)) unsigned short  WORD;
*/
typedef char            CHAR;
typedef unsigned char   UCHAR;
typedef unsigned short  WORD;

struct MS3DVertex {
  UCHAR  editorFlags; // nie uzywane
  float  position[3];
  CHAR   boneIndice;
  UCHAR  referenceCount; // nie uzywane
};

struct MS3DFace {
  WORD  editorFlags;
  WORD  verticesIndices[3];
  float verticesNormals[3][3];
  float textureCoords[2][3];
  UCHAR  smoothingGroup;
  UCHAR  groupIndex;
};

struct MS3DMesh {
  UCHAR editorFlags;
  CHAR  name[32];
  WORD  triNum;
};

struct MS3DMaterial {
  CHAR  name[32];
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float emissive[4];
  float shininess;    // 0 - 128
  float transparency; // 0 - 1
  CHAR  mode;         // nie uzywane
  CHAR  texture[128]; // nazwa pliku tekstury
  CHAR  alpha[128];   // nazwa pliku wartosci alpha
};

struct MS3DJoint {
  UCHAR  editorFlags;
  CHAR   name[32];
  CHAR   parentName[32];
  float  initialRotation[3];
  float  initialPosition[3];
  WORD   numOfKeyFramesRotation;
  WORD   numOfKeyFramesTranslation;
};

struct MS3DKeyFrame {
  float time;
  float param[3];
};

void
formatValidation(std::istream& inputData)
{
  char strIdentifier[10] = {0};
  readFromStdStream( inputData,
    strIdentifier,
    sizeof(strIdentifier) );
  
  int versionInfo = 0;
  readFromStdStream( inputData,
    &versionInfo,
    1 );
  
  if( string(strIdentifier, 10) != "MS3D000000" ||
    !inputData ||
    !(versionInfo == 3 || versionInfo == 4) )
  {
    throw MS3DModel::BadDataFormat( 
      "Proba odczytu blednego formatu danych MS3D!" );
  }
}

void
extractVertices(istream& inputData,
                MS3DModel::Vertices& vertices)
{
  WORD verticesNum;
  readFromStdStream( inputData,
    &verticesNum,
    1 );

// Powinno wynosic 15, ale kompilator wyrownuje do 20! :\
// size_t a=sizeof(MS3DVertex); 
  
  RefArrayCntPtr<MS3DVertex> verticesDataChunk(new MS3DVertex[verticesNum]);
  
  for(int i = 0; i < verticesNum; ++i) {
    readFromStdStream( inputData,
      &verticesDataChunk[i].editorFlags, 
      1);
    readFromStdStream( inputData,
      &verticesDataChunk[i].position, 
      1);
    readFromStdStream( inputData,
      &verticesDataChunk[i].boneIndice,
      1);
    readFromStdStream( inputData,
      &verticesDataChunk[i].referenceCount,
      1);
  }

  /*readFromStdStream( inputData,
    verticesDataChunk.get(),
    verticesNum);*/

  vertices.reserve(verticesNum);
    
  typedef MS3DModel::Vertex Vertex;
  for(int i = 0; i < verticesNum; ++i) {
    Vertex vertex(verticesDataChunk[i].position[0],
                  verticesDataChunk[i].position[1],
                  verticesDataChunk[i].position[2],
                  verticesDataChunk[i].boneIndice);
    vertices.push_back(vertex);
  }
}

void
extractFaces(istream& inputData,
             MS3DModel::Faces& faces)
{
  WORD facesNum;
  readFromStdStream( inputData,
    &facesNum,
    1 );

  RefArrayCntPtr<MS3DFace> facesDataChunk(new MS3DFace[facesNum]);

  for(int i = 0; i < facesNum; ++i) {
    readFromStdStream( inputData,
      &facesDataChunk[i].editorFlags, 
      1);
    readFromStdStream( inputData,
      &facesDataChunk[i].verticesIndices, 
      1);
    readFromStdStream( inputData,
      &facesDataChunk[i].verticesNormals, 
      1);
    readFromStdStream( inputData,
      &facesDataChunk[i].textureCoords, 
      1);
    readFromStdStream( inputData,
      &facesDataChunk[i].smoothingGroup, 
      1);
    readFromStdStream( inputData,
      &facesDataChunk[i].groupIndex, 
      1);  
  }

  /*readFromStdStream( inputData,
    facesDataChunk.get(),
    facesNum );*/

  faces.reserve(facesNum);
    
  typedef MS3DModel::Face Face;
  typedef Face::Normal    Normal;
  typedef Face::TexCoord  TexCoord;
  for(int i = 0; i < facesNum; ++i) {
    Face polygon;
    for(int j = 0; j < 3; ++j) {
      MS3DFace& currFace = facesDataChunk[i];
      polygon.setVertexIndice(j, currFace.verticesIndices[j]);
      polygon.setVertexNormal(j, Normal(
        currFace.verticesNormals[j][0],
        currFace.verticesNormals[j][1],
        currFace.verticesNormals[j][2] )
      );
      
      // Koordynaty tekstur formatu MS3D sa ulozone w nastepujacy sposob:
      // u1, u2, u3, v1, v2, v3
      polygon.setVertexTexCoord(j, TexCoord(
        currFace.textureCoords[0][j],
        currFace.textureCoords[1][j] )
      );
    }
    faces.push_back(polygon);
  }
}

void
extractMeshes(istream& inputData,
              MS3DModel::Meshes& meshes)
{
  WORD meshesNum;
  readFromStdStream( inputData,
    &meshesNum,
    1 ); 

  meshes.resize(meshesNum);

  for(int meshCnt = 0; meshCnt < meshesNum; ++meshCnt) {
    MS3DMesh ms3dMesh;

    readFromStdStream(inputData,
      &ms3dMesh.editorFlags,
      1);
    readFromStdStream(inputData,
      &ms3dMesh.name,
      1);
    readFromStdStream(inputData,
      &ms3dMesh.triNum,
      1);
  
    /*readFromStdStream(inputData,
      &ms3dMesh,
      1);*/

    typedef WORD MS3DTriIndex;
    RefArrayCntPtr<MS3DTriIndex> triIndices(new MS3DTriIndex[ms3dMesh.triNum]);
    readFromStdStream(inputData,
      triIndices.get(),
      ms3dMesh.triNum);

    meshes[meshCnt].setTriangleIndices(ms3dMesh.triNum, triIndices.get());

    char materialIndex;
    readFromStdStream(inputData,
      &materialIndex,
      1);

    meshes[meshCnt].setMaterial(materialIndex);
  }
}

void
extractMaterials(istream& inputData,
                 MS3DModel::Materials& materials)
{
  WORD materialsNum;
  readFromStdStream( inputData,
    &materialsNum,
    1 );

  RefArrayCntPtr<MS3DMaterial>  ms3dMaterials(new MS3DMaterial[materialsNum]);
  
  for(int i = 0; i < materialsNum; ++i) {
    readFromStdStream( inputData,
      &ms3dMaterials[i].name,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].ambient,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].diffuse,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].specular,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].emissive,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].shininess,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].transparency,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].mode,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].texture,
      1);
    readFromStdStream( inputData,
      &ms3dMaterials[i].alpha,
      1);
  }
 
  /*readFromStdStream( inputData,
    ms3dMaterials.get(),
    materialsNum);*/
    
  typedef MS3DModel::Material    Material;
  typedef Material::RGBIntensity Rgb;
  for(int materialCounter = 0; 
    materialCounter < materialsNum;
    ++materialCounter)
  {
    MS3DMaterial& currMat = ms3dMaterials[materialCounter];

    materials.push_back( Material( 
      Rgb(currMat.ambient[0], currMat.ambient[1], currMat.ambient[2]),
      Rgb(currMat.diffuse[0], currMat.diffuse[1], currMat.diffuse[2]),
      Rgb(currMat.specular[0], currMat.specular[1], currMat.specular[2]),
      Rgb(currMat.emissive[0], currMat.emissive[1], currMat.emissive[2]),
      currMat.shininess,
      static_cast<bool>(currMat.transparency),
      currMat.texture,
      currMat.alpha )
    );
  }
}

void
extractJoints(istream& inputData,
              MS3DModel::Joints& joints)
{
  WORD jointsNum;
  readFromStdStream( inputData,
    &jointsNum,
    1 );
  
  joints.reserve(jointsNum);

  for(size_t jointIndex = 0; jointIndex < jointsNum; ++jointIndex)
  {
    RefCountPtr<MS3DJoint> ms3dJoint(new MS3DJoint);

    readFromStdStream(inputData,
      &ms3dJoint->editorFlags,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->name,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->parentName,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->initialRotation,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->initialPosition,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->numOfKeyFramesRotation,
      1);
    readFromStdStream(inputData,
      &ms3dJoint->numOfKeyFramesTranslation,
      1);

    /*readFromStdStream(inputData,
      ms3dJoint.get(),
      1);*/

    typedef MS3DModel::Joint Joint;
    typedef Joint::Vector3   Vector3;
    
    joints.push_back( Joint(
      inputData,
      ms3dJoint->name,
      ms3dJoint->parentName,
      Vector3(ms3dJoint->initialRotation[0],
              ms3dJoint->initialRotation[1],
              ms3dJoint->initialRotation[2]),
      Vector3(ms3dJoint->initialPosition[0],
              ms3dJoint->initialPosition[1],
              ms3dJoint->initialPosition[2]),
      ms3dJoint->numOfKeyFramesRotation,
      ms3dJoint->numOfKeyFramesTranslation )
    );
  }
}

} // anonymous namespace

namespace Gcad {
namespace Framework {

MS3DModel
::MS3DModel(std::istream& inputData)
{
  formatValidation(inputData);
  extractVertices(inputData, vertices_);
  extractFaces(inputData, faces_);
  extractMeshes(inputData, meshes_);
  extractMaterials(inputData, materials_);
  extractJoints(inputData, joints_);
}

MS3DModel::Face::Normal 
MS3DModel::Face
::getNormal(int whichVertex) const 
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");

  return normals_[whichVertex];
}

MS3DModel::Face::TexCoord
MS3DModel::Face
::getTexCoord(int whichVertex) const 
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");

  return texCoords_[whichVertex];
}

MS3DModel::Face::VertexIndice
MS3DModel::Face
::getVerticeIndex(int whichVertex) const 
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");
  
  return verticesIndices_[whichVertex];
}

void 
MS3DModel::Face
::setVertexNormal(int whichVertex, 
                  const Normal& normal)
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");
  normals_[whichVertex] = normal;
}

void 
MS3DModel::Face
::setVertexTexCoord(int whichVertex, 
                    const TexCoord& texCoord)
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");
  texCoords_[whichVertex] = texCoord;
}

void 
MS3DModel::Face
::setVertexIndice(int whichVertex, 
                  VertexIndice indice)
{
  Utilities::assertion(whichVertex >= 0 && whichVertex <=3,
    "Trojkat ma trzy wierzcholki! Proba pobrania wartosci "
    "spoza zakresu!");
  verticesIndices_[whichVertex] = indice;
}

void 
MS3DModel::Mesh
::setTriangleIndices( int indicesCount, 
                      TriangleIndice* indiceData )
{
  Utilities::assertion(indiceData != 0,
    "Przekazano bledne dane podczas tworzenia 'wskaznikow' do poligonow "
    "uzywanych przez obiekt siatki!");

  triIndexes_.resize(indicesCount);
  std::copy(indiceData, indiceData + indicesCount,
    &triIndexes_.front());
}


MS3DModel::Joint
::Joint(std::istream& input,
        const std::string& name,
        const std::string& parentName,
        const Vector3& initialRotation,
        const Vector3& initialPosition,
        int numKeyFrameRotations,
        int numKeyFrameTranslations)
  : name_(name)
  , parentName_(parentName)
  , initialRotation_(initialRotation)
  , initialPosition_(initialPosition)
{
  const int KFR = numKeyFrameRotations;
  
  RefArrayCntPtr<MS3DKeyFrame>  ms3dKeyFrameRotate(new MS3DKeyFrame[KFR]);

  for(int i = 0; i < KFR; ++i) {
    readFromStdStream(input,
      &ms3dKeyFrameRotate[i].time,
      1);
    readFromStdStream(input,
      &ms3dKeyFrameRotate[i].param,
      1);
  }

  /*readFromStdStream(input,
    ms3dKeyFrameRotate.get(),
    KFR);*/

  keyFrameTimeForRotation_.reserve(KFR);
  keyFrameRotation_.reserve(KFR);
  keyFrameQuaternion_.reserve(KFR);

  for(int keyFrameRotIndex = 0;
    keyFrameRotIndex < KFR;
    ++keyFrameRotIndex)
  {
    keyFrameTimeForRotation_.push_back(
      ms3dKeyFrameRotate[keyFrameRotIndex].time);

    Vector3 eulerAngles( 
      ms3dKeyFrameRotate[keyFrameRotIndex].param[0],
      ms3dKeyFrameRotate[keyFrameRotIndex].param[1],
      ms3dKeyFrameRotate[keyFrameRotIndex].param[2] );
    
    keyFrameRotation_.push_back(eulerAngles);

    Quaternion keyFrameQuaternionRotation;
    eulerToQuaternion(eulerAngles, &keyFrameQuaternionRotation);
    keyFrameQuaternion_.push_back(keyFrameQuaternionRotation);
  }

  const int KFT = numKeyFrameTranslations;

  RefArrayCntPtr<MS3DKeyFrame>  ms3dKeyFrameTransl(new MS3DKeyFrame[KFR]);

  for(int i = 0; i < KFR; ++i) {
    readFromStdStream(input,
      &ms3dKeyFrameTransl[i].time,
      1);
    readFromStdStream(input,
      &ms3dKeyFrameTransl[i].param,
      1);
  }

 /*readFromStdStream(input,
    ms3dKeyFrameTransl.get(),
    KFT);*/

  keyFrameTimeForPosition_.reserve(KFT);
  keyFramePosition_.reserve(KFT);

  for(int keyFrameTransIndex = 0;
    keyFrameTransIndex < KFT;
    ++keyFrameTransIndex)
  {
    keyFrameTimeForPosition_.push_back(
      ms3dKeyFrameTransl[keyFrameTransIndex].time);

    keyFramePosition_.push_back(
      Vector3( ms3dKeyFrameTransl[keyFrameTransIndex].param[0],
               ms3dKeyFrameTransl[keyFrameTransIndex].param[1],
               ms3dKeyFrameTransl[keyFrameTransIndex].param[2] )
    );
  }

  eulerToQuaternion(initialRotation_, &initQuatRotation_);
}

float 
MS3DModel::Joint
::getRotationFrameTimeMember(size_t whichFrame) const 
{
  Utilities::assertion(whichFrame >= 0 &&
    whichFrame < getRotationKeyFrameNum(),
    "Wartosc indeksu spoza zakresu!");
    
  return keyFrameTimeForRotation_[whichFrame];
}

MS3DModel::Joint::Vector3 
MS3DModel::Joint
::getRotationFrameData(size_t whichFrame) const 
{ 
  Utilities::assertion(whichFrame >= 0 &&
    whichFrame < getRotationKeyFrameNum(),
    "Wartosc indeksu spoza zakresu!");
  
  return keyFrameRotation_[whichFrame];
}

MS3DModel::Joint::Quaternion 
MS3DModel::Joint
::getQuatRotationFrameData(size_t whichFrame) const
{
  Utilities::assertion(whichFrame >= 0 &&
    whichFrame < getRotationKeyFrameNum(),
    "Wartosc indeksu spoza zakresu!");
  
  return keyFrameQuaternion_[whichFrame];
}

float 
MS3DModel::Joint
::getTranslationFrameTimeMember(size_t whichFrame) const 
{ 
  Utilities::assertion(whichFrame >= 0 &&
    whichFrame < getTranslationKeyFrameNum(),
    "Wartosc indeksu spoza zakresu!");
  
  return keyFrameTimeForPosition_[whichFrame];
}

MS3DModel::Joint::Vector3 
MS3DModel::Joint
::getTranslationFrameData(size_t whichFrame) const 
{ 
  Utilities::assertion(whichFrame >= 0 &&
    whichFrame < getTranslationKeyFrameNum(),
    "Wartosc indeksu spoza zakresu!");
  
  return keyFramePosition_[whichFrame];
}

const MS3DModel::Vertex&
MS3DModel
::getVertex(size_t vertexIndex) const 
{
  Utilities::assertion(vertexIndex < getVerticesNum(),
    "Indeks wierzcholka spoza zakresu!");
  return vertices_[vertexIndex];
}

const MS3DModel::Face& 
MS3DModel
::getFace(size_t faceIndex) const 
{
  Utilities::assertion(faceIndex < getFacesNum(),
    "Indeks poligonu spoza zakresu!");
  return faces_[faceIndex];
}

const MS3DModel::Mesh& 
MS3DModel
::getMesh(size_t meshIndex) const 
{
  Utilities::assertion(meshIndex < getMeshesNum(),
    "Indeks siatki spoza zakresu!");
  return meshes_[meshIndex];
}

const MS3DModel::Material& 
MS3DModel
::getMaterial(size_t materialIndex) const 
{
  Utilities::assertion(materialIndex < getMaterialsNum(),
    "Indeks materialu spoza zakresu!");
  return materials_[materialIndex];
}

const MS3DModel::Joint& 
MS3DModel
::getJoint(size_t jointIndex) const 
{
  Utilities::assertion(jointIndex < getJointsNum(),
    "Indeks okreslajacy joint'a spoza zakresu!");
  return joints_[jointIndex];
}

} // namespace Framework
} // namespace Gcad

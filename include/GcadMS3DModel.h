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

#ifndef _GCAD_MS3DMODEL_H_
#define _GCAD_MS3DMODEL_H_

#include "GcadAssertion.h"
#include "GcadException.h"
#include "GcadQuaternion.h"
#include "GcadVector2.h"
#include "GcadVector3.h"
#include <iostream>
#include <string>
#include <vector>

namespace Gcad {
namespace Framework {

/**
  @brief
    Klasa wspomagajaca odczyt danych formatu MilkShape3D dla wersji 3 i 4.
    Wiecej informacji mozna znalezc w publikacji "Focus on 3D Models"
   
    Wierzcholki, z ktorych sklada sie model. W przeciwienstwie do formatow
    typu MD2, MD3 (idSoftware), ktore opieraja animacje na technice
    klatek kluczowych, format MS3D korzysta z metody "animacji szkieletowej".
    Dzieki takiemu rozwiazaniu, uzyskujemy szereg udogodnien, oraz
    zmniejszamy zapotrzebowanie na dane wierzcholkow kazdej klatki kluczowej
    animacji. Jedynym mankamentem tej techniki jest jej zlozonosc, nie mniej 
    jednak w pelni rekompensowana przez uzyskiwana elastycznosc

  @code
    #include "GcadMS3DModel.h"
    #include <fstream>
    using namespace std;
    using namespace Gcad::Framework;

    int main(int argc, char** argv) {
      fstream   dataModel(argv[1], ios_base::binary);
      MS3DModel ms3dModel(dataModel);      
      // now you can use internal methods of component, to get
      // an specyfic data resources. just remember, opened
      // stream must be binary type!
    }
  @endcode
*/
class GCAD_EXPORT MS3DModel {
 public:
   class GCAD_EXPORT Vertex;
   typedef std::vector<Vertex>       Vertices;
   typedef Vertices::const_iterator  VerticesConstItor;

   class GCAD_EXPORT Face;
   typedef std::vector<Face>      Faces; 
   typedef Faces::const_iterator  FacesConstItor;

   class GCAD_EXPORT Mesh;
   typedef std::vector<Mesh>       Meshes;
   typedef Meshes::const_iterator  MeshesConstItor;

   class GCAD_EXPORT Material;
   typedef std::vector<Material>      Materials;
   typedef Materials::const_iterator  MaterialsConstItor;

   class GCAD_EXPORT Joint;
   typedef std::vector<Joint>      Joints;
   typedef Joints::const_iterator  JointsConstItor;

 public:
   /**
     @brief
       Wierzcholek modelu MS3D, ktory zawiera informacje o polozeniu
       przestrzennym, oraz wartosc determinujaca dopasowanie do
       JEDNEJ "kosci" (istnieja rowniez modele, ktorych wierzcholki
       moga nalezec do wiekszej liczby kosci; ms3d release 3, lub 4 
       nie wydaje sie posiadac takiej opcji)
   */
   class GCAD_EXPORT Vertex {
    public:
      typedef Math::Vector3<float>  Position;
      typedef char                  BoneIndice;

    public:
      Position    getPosition() const { return pos_; }
      BoneIndice  getBoneIndice() const { return boneIndice_; }
      bool        isAttachedToBone() const { return boneIndice_ != -1; }

    public:
      // Interfejs wykorzystywany przez wewnetrzna implementacje
      // W zasadzie nie powinien nalezec do publicznego uzytku, jednak
      // skorzystanie z technik, w ktorej korzysta sie z anonimowej
      // przestrzeni, uniemozliwia hermetyzacje interfejsu.
      // Konkluzja - klient nie powinien wykonywac zadnego z konstruktorow
      Vertex()
        : boneIndice_(0)
      {}

      Vertex(float x, float y, float z, BoneIndice boneIndice)
        : pos_(x, y, z)
        , boneIndice_(boneIndice)
      {}

    private:
      Position    pos_;        /**< @brief Pozycja przestrzenna */
      BoneIndice  boneIndice_; /**< @brief Indeks do tablicy kosci */
   };

   /**
     @brief
       Informacje determinujace jeden poligon siatki
       
       W sklad kazdego elementu trojkata (trzy skladowe, z ktorych zbudowany 
       jest trojkat) skomponowany jest z aproksymowanego wektora normalnego 
       (normals_[0..2]) - przypisanego wierzcholkowi o zadanym indeksie 
       (verticesIndices_[0..2]) - oraz z koordynat tekstur (texCoords_[0..2]) 
       odpowiednio przypisanych kolejnym wierzcholkom
   */
   class GCAD_EXPORT Face {
    public:
      typedef Math::Vector3<float>  Normal;
      typedef Math::Vector2<float>  TexCoord;
      typedef unsigned short        VertexIndice;
    
    public:
      // Wartosci wejsciowe okreslone zmienna whichVertex 
      // musza nalezec do przedzialu 0..2 (trzy wierzcholki trojkata/poligonu). 
      // Nie dostosowanie sie do tego warunku "zaowocuje" bledem asercji
      Normal       getNormal(int whichVertex) const;
      TexCoord     getTexCoord(int whichVertex) const;
      VertexIndice getVerticeIndex(int whichVertex) const;

    public:
      // Interfejs wykorzystywany przez wewnetrzna implementacje.
      // Klient nie powinien korzystac z tej funkcjonalnosci, poniewaz
      // moze doprowadzic do niespojnosci danych komponentu
      void setVertexNormal(int whichVertex, const Normal& normal);
      void setVertexTexCoord(int whichVertex, const TexCoord& texCoord);
      void setVertexIndice(int whichVertex, VertexIndice indice);

    private:
      Normal normals_[3];       /**< @brief Normalne wierzcholkow trojkata */
      TexCoord texCoords_[3];   /**< @brief Koordynaty tekstur wierzcholkow */
      VertexIndice verticesIndices_[3]; /**< @brief Wskazniki na dane pozycji
                                             wierzcholkow */
   };

   /**
     @brief
       Definicja siatki modelu, czyli danych wskazujacych na z gory
       okreslone (np. w programie do modelowania obiektow 3D) grupy poligonow
       (podzbior calego zestawu danych MS3DModel::Faces faces_), do ktorych 
       przypisany jest odpowiedni material (wartosc indeksu wskazujacego na 
       element w tablicy materialow MS3DModel::Materials)
   */
   class GCAD_EXPORT Mesh {
    public:
      typedef unsigned short                   TriangleIndice;
      typedef std::vector<TriangleIndice>      TriangleIndexes;
      typedef TriangleIndexes::const_iterator  TriangleIndexesConstItor;
      typedef char                             MaterialIndex;

    public:
      bool          hasMaterial() const { return matIndex_ != -1; }
      MaterialIndex getMaterialIndex() const { return matIndex_; }
      size_t        getTriIndicesNum() const { return triIndexes_.size(); }

      TriangleIndexesConstItor beginTriangleIndexes() const {
        return triIndexes_.begin();
      }
      TriangleIndexesConstItor endTriangleIndexes() const {
        return triIndexes_.end();
      }

    public:
      // Interfejs implementacji wewnetrznej. Klient nie jest upowazniony
      // do jego uzywania!
      void setTriangleIndices( int indicesCount, 
                               TriangleIndice* indiceData );

      void setMaterial( MaterialIndex matIndice ) { matIndex_ = matIndice; }
    
    private:
      TriangleIndexes  triIndexes_;
      MaterialIndex    matIndex_;
   };

   /**
     @brief
       Dane materialu, ktory okresla wspolczynniki absorbcji swiatel:
       otoczenia, rozproszenia, odbitego, emisyjnego. Pozostale skladowe
       determinuja polyskliwosc, przezroczystosc, oraz nazwy plikow
       zawierajacych dane tekstury, oraz dane alfa.
   */
   class GCAD_EXPORT Material {
    public:
      typedef Math::Vector3<float> RGBIntensity;

    public:
      RGBIntensity getAmbient() const { return ambient_; }
      RGBIntensity getDiffuse() const { return diffuse_; }
      RGBIntensity getSpecular() const { return specular_; }
      RGBIntensity getEmissive() const { return emissive_; }
      float        getShininess() const { return shininess_; }
      bool         getTransparency() const { return transparency_; }
      
      const std::string& getTextureName() const { return textureName_; }
      const std::string& getAlphaTextureName() const { return alphaTexName_; }

    public:
      // Jak i w poprzednich przypadkach funkcje zawarte w tym
      // przedziale, oznaczonym przez specyfikator publiczny nie powinny
      // byc uzywane przez klienta korzystajacego z udostepnianych uslug
      Material()
        : shininess_(0.0f)
        , transparency_(false)
      {}

      Material(RGBIntensity ambient,
               RGBIntensity diffuse,
               RGBIntensity specular,
               RGBIntensity emissive,
               float shininess,
               bool transparency,
               const char* textureName,
               const char* alphaTexName)
        : ambient_(ambient)
        , diffuse_(diffuse)
        , specular_(specular)
        , emissive_(emissive)
        , shininess_(shininess)
        , transparency_(transparency)
        , textureName_(textureName)
        , alphaTexName_(alphaTexName)
      {}   
     
    private:
      RGBIntensity ambient_;
      RGBIntensity diffuse_;
      RGBIntensity specular_;
      RGBIntensity emissive_;
      float        shininess_;
      bool         transparency_;
      std::string  textureName_;
      std::string  alphaTexName_;
   };

   /**
     @brief
       Definicja joint'a, czyli glownego obiektu odpowiedzialnego za sterowanie
       animacja calej geometrii modelu. Kazdy joint posiada informacje, ktore 
       umozliwiaja wykonanie animacji, utworzonej w programie modelowania 3d.
       Zbior joint'ow tworzy tzw. szkielet modelu, ktory wplywa na podstawowy
       zestaw wierzcholkow. Hierarchiczna struktura, dzieki ktorej mozliwe jest 
       wykonywanie animacji poszczegolnych czesci modelu, jest oparta na 
       drzewie, ktorego wezlami sa joint'y. Wykonanie trawersacji poprzez 
       elementy wchodzace w sklad szkieletu umozliwia animacje modelu, dzieki
       wyliczaniu zlozen (konkatencji) poszczegolnych przeksztalcen 
       zdefiniowanych w sprzezonych joint'ach. Zapis obrotow w formie 
       kwaternionow (po konwersji katow Eulera) sprzyja plynnej animacji, oraz
       wymaga mniej zasobow, niz przechowywanie danych w postaci macierzy
   */
   class GCAD_EXPORT Joint {
    public:
      typedef Math::Vector3<float>    Vector3;
      typedef Math::Quaternion<float> Quaternion;
      
      // Z kazdym joint'em zwiazany jest zestaw struktur (wartosci), ktore
      // determinuja kolejne klatki animacji. Tutaj uwidacznia sie zmiana
      // wzgledem formatow opartych na klatkach kluczowych. Mowiac klatki 
      // animacji mam na mysli ujecia, ktore definiowane sa w kategoriach 
      // "kosci", czyli przeksztalcen typu obrot (w tym wypadku okreslony
      // katami Eulera), oraz translacje.
      // Rotacja, oraz translacja skladaja sie z (kazda z osobna): trzech
      // wartosci float (KeyFrameParam) - ktore dla rotacji oznaczaja
      // katy Eulera odpowiednio dla osi XYZ, natomiast dla translacji
      // oznaczaja wektor przesuniecia - oraz wartosci time, ktora determinuje
      // ilosc czasu (przedzial, delte czasu) potrzebnego do wykonania
      // pelnej animacji danego ujecia
      typedef std::vector<Vector3>    KeyFrameParam;
      typedef std::vector<Quaternion> KeyFrameQuaternion;
      typedef std::vector<float>      KeyFrameTime;

    public:
      const std::string& getName() const { return name_; }
      const std::string& getParentName() const { return parentName_; }
      
      Vector3    getInitialOrientation() const { return initialRotation_; }
      Quaternion getInitialQuatOrient() const { return initQuatRotation_; }
      Vector3    getInitialPosition() const { return initialPosition_; }
      float      getInitialXEulerAngle() const { return initialRotation_.x(); }
      float      getInitialYEulerAngle() const { return initialRotation_.y(); }
      float      getInitialZEulerAngle() const { return initialRotation_.z(); }

      size_t getRotationKeyFrameNum() const { return keyFrameRotation_.size(); }
      size_t getTranslationKeyFrameNum() const {return keyFramePosition_.size();}

      float      getRotationFrameTimeMember(size_t whichFrame) const;
      Vector3    getRotationFrameData(size_t whichFrame) const;
      Quaternion getQuatRotationFrameData(size_t whichFrame) const;

      float   getTranslationFrameTimeMember(size_t whichFrame) const;
      Vector3 getTranslationFrameData(size_t whichFrame) const;

    public:
      // Interfejs wewnetrznej implementacji. Klient nie jest upowazniony
      // do wykonywania funkcji zawartych w tym przedziale!      
      
      Joint(std::istream& input,
            const std::string& name,
            const std::string& parentName,
            const Vector3& initialRotation,
            const Vector3& initialPosition,
            int numKeyFrameRotations,
            int numKeyFrameTranslations);

    private:
      std::string name_;  /**< @brief Nazwa joint'a zdefiniowana w edytorze */
      std::string parentName_; /**< @brief Nazwa joint'a wezla ojca */
      
      Vector3 initialRotation_; /**< @brief Skladowe XYZ (wartosci katow Eulera)
                                            wyjsciowej orientacji modelu */
      Vector3 initialPosition_; /**< @brief Wyjsciowa pozycja modelu */
      
      /** @brief Poczatkowa orientacja "kosci" wyrazona kwaternionem */
      Quaternion initQuatRotation_;

      // Dane determinujace kluczowe klatki orientacji joint'a modelu
      KeyFrameTime        keyFrameTimeForRotation_;
      KeyFrameParam       keyFrameRotation_;
      KeyFrameQuaternion  keyFrameQuaternion_;
      
      // Dane determinujace kluczowe klatki pozycji joint'a modelu
      KeyFrameTime  keyFrameTimeForPosition_;
      KeyFrameParam keyFramePosition_;       
   };

   /**
     @brief
       Klasa wyjatku wyrzucanego podczas proby odczytu danych
       niezgodnych z konwencja zapisu formatu MS3D (wystapienie
       blednego identyfikatora poprzedzajacego dane geometrii)
   */
   struct GCAD_EXPORT BadDataFormat : Utilities::Exception {
     BadDataFormat(const std::string& exceptionDescription)
       : Utilities::Exception(exceptionDescription)
     {}
   };

 public:
   MS3DModel(std::istream& inputData);

 public: // interfejs bazujacy na iteratorach
   VerticesConstItor beginVertices() const { return vertices_.begin(); }
   VerticesConstItor endVertices() const { return vertices_.end(); }

   FacesConstItor beginFaces() const { return faces_.begin(); }
   FacesConstItor endFaces() const { return faces_.end(); }

   MeshesConstItor beginMeshes() const { return meshes_.begin(); }
   MeshesConstItor endMeshes() const { return meshes_.end(); }

   MaterialsConstItor beginMaterials() const { return materials_.begin(); }
   MaterialsConstItor endMaterials() const { return materials_.end(); }

   JointsConstItor beginJoints() const { return joints_.begin(); }
   JointsConstItor endJoints() const { return joints_.end(); }

 public: // interfejs bazujacy na operacjach subskrypcji
   size_t getVerticesNum() const { return vertices_.size(); }
   size_t getFacesNum() const { return faces_.size(); }
   size_t getMeshesNum() const { return meshes_.size(); }
   size_t getMaterialsNum() const { return materials_.size(); }
   size_t getJointsNum() const { return joints_.size(); }

   const Vertex&    getVertex(size_t vertexIndex) const;
   const Face&      getFace(size_t faceIndex) const;
   const Mesh&      getMesh(size_t meshIndex) const;
   const Material&  getMaterial(size_t materialIndex) const;
   const Joint&     getJoint(size_t jointIndex) const;

 private:
   Vertices   vertices_;  /**< @brief Wierzcholki tworzace model */
   Faces      faces_;     /**< @brief Zestawy danych poligonow modelu */
   Meshes     meshes_;    /**< @brief Komponenty modelu (siatki) */
   Materials  materials_; /**< @brief Materialy modelu */
   Joints     joints_;    /**< @brief Szkielet animacyjny */
};

} // namespace Framework
} // namespace Gcad

#endif

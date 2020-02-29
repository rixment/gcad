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

#include "GcadSymbolTST.h"
#include "GcadStdIos.h"
#include "GcadMD2Data.h"
#include "GcadDriveElementsEnumeratorWin32.h"
#include <fstream>
#include <memory>
#include <iomanip>
#include <strstream>
#include <string>

using namespace std;
using namespace Gcad::Framework;
using namespace Gcad::Platform;
using namespace Gcad::Utilities;

class MD2DataConverter;
typedef RefCountPtr<MD2DataConverter> TranslatorRefPtr;

class MD2DataConverter {
 public:
   virtual ~MD2DataConverter() {
   }
   void execute(const MD2Data& md2Data, const string& fileName) {
     init(md2Data, fileName);
     doAttributes();
     doVerticesFrames();
     doNormalsFrames();
     doTexturesCoords();
     doPolygonIndices();
   }
 protected:
   virtual void init(const MD2Data&, const string&) = 0;
   virtual void doAttributes() = 0;
   virtual void doVerticesFrames() = 0;
   virtual void doNormalsFrames() = 0;
   virtual void doTexturesCoords() = 0;
   virtual void doPolygonIndices() = 0;
};

/**
  @brief
    Specify special binary format output
*/
class MD2DataConverterToSH2Format : public MD2DataConverter {
   const MD2Data*  md2Data_;
   ofstream        outputStream_;
 protected:
   virtual void init(const MD2Data& md2Data, const string&  outputFileName) {
     md2Data_ = &md2Data;
     outputStream_.open(outputFileName.c_str(), ios_base::binary);
   }
   virtual void doAttributes() {
     writeIntoStdStream(outputStream_, md2Data_->attributes().numFrames(), 1);
     writeIntoStdStream(outputStream_, md2Data_->attributes().numVerts(), 1);
     writeIntoStdStream(outputStream_, md2Data_->attributes().numPolys(), 1);
     writeIntoStdStream(outputStream_, md2Data_->attributes().numTexCoords(), 1);
   }
   virtual void doVerticesFrames() {
     for(size_t keyFrameIndex = 0;
       keyFrameIndex < md2Data_->attributes().numFrames();
       ++keyFrameIndex)
     {
       writeIntoStdStream(outputStream_, 
         *md2Data_->beginVerticesKeyFrame(keyFrameIndex),
         md2Data_->attributes().numVerts());
     }
   }
   virtual void doNormalsFrames()  {
     for(size_t keyFrameIndex = 0;
       keyFrameIndex < md2Data_->attributes().numFrames();
       ++keyFrameIndex)
     {
       writeIntoStdStream(outputStream_, 
         *md2Data_->beginNormalsKeyFrame(keyFrameIndex),
         md2Data_->attributes().numVerts());
      }
   }
   virtual void doTexturesCoords() {
     writeIntoStdStream(outputStream_, 
       *md2Data_->beginTextureCoords(),
       md2Data_->attributes().numTexCoords());
   }
   virtual void doPolygonIndices() {
     writeIntoStdStream(outputStream_, 
       *md2Data_->beginPolygonsIndices(),
       md2Data_->attributes().numPolys());
   }
};

/**
  @brief
    Class describes an output text format
*/
class MD2DataConverterToPlainText : public MD2DataConverter {
   const MD2Data*  md2Data_;
   ofstream        outputStream_;
   const streamsize  FLOAT_SETW; // used to give an nice output text format
   const streamsize  INT_SETW;   // used to give an nice output text format
 public:
   MD2DataConverterToPlainText() 
     : FLOAT_SETW(7 + outputStream_.precision())
     , INT_SETW(5) 
   {}
 protected:
   virtual void init(const MD2Data& md2Data, const string&  outputFileName) {
     md2Data_ = &md2Data;
     outputStream_.open(outputFileName.c_str());
   }
   virtual void doAttributes() {
     const streamsize S = 40;
     outputStream_ << setw(S) << "FRAMES NUMBER = " << 
       md2Data_->attributes().numFrames() << endl;
     outputStream_ << setw(S) << "VERTICES NUMBER PER FRAME = " << 
       md2Data_->attributes().numVerts() << endl;
     outputStream_ << setw(S) << "POLYGONS NUMBER PER MESH = " <<
       md2Data_->attributes().numPolys() << endl;
     outputStream_ << setw(S) << "TEXTURE COORDS PER MESH = " << 
       md2Data_->attributes().numTexCoords() << endl;
   }
   virtual void doVerticesFrames() {
     outputStream_ << "\nVERTICES DATA:\n" << endl;
     for(size_t keyFrameIndex = 0;
       keyFrameIndex < md2Data_->attributes().numFrames();
       ++keyFrameIndex)
     {
       outputStream_ << "FRAME NUM: " << keyFrameIndex << endl;
       typedef MD2Data::VerticesFrameConstItor VertexItor;
       for(VertexItor it = md2Data_->beginVerticesKeyFrame(keyFrameIndex);
         it != md2Data_->endVerticesKeyFrame(keyFrameIndex);
         ++it)
       {
         outputStream_ << 
           setw(FLOAT_SETW) << it->x() << " " << 
           setw(FLOAT_SETW) << it->y() << " " << 
           setw(FLOAT_SETW) << it->z() << endl;
       }
     }
   }
   virtual void doNormalsFrames()  {
     outputStream_ << "\nNORMALS DATA:\n" << endl;
     for(size_t keyFrameIndex = 0;
       keyFrameIndex < md2Data_->attributes().numFrames();
       ++keyFrameIndex)
     {
       outputStream_ << "FRAME NUM: " << keyFrameIndex << endl;
       typedef MD2Data::NormalsFrameConstItor VertexItor;
       for(VertexItor it = md2Data_->beginNormalsKeyFrame(keyFrameIndex);
         it != md2Data_->endNormalsKeyFrame(keyFrameIndex);
         ++it)
       {
         outputStream_ << 
           setw(FLOAT_SETW) << it->x() << " " << 
           setw(FLOAT_SETW) << it->y() << " " << 
           setw(FLOAT_SETW) << it->z() << endl;
       }
     }
   }
   virtual void doTexturesCoords() {
     outputStream_ << "\nTEXTURE COORDINATES DATA:\n" << endl;
     for(MD2Data::TextureCoordsConstItor it = md2Data_->beginTextureCoords();
       it != md2Data_->endTextureCoords();
       ++it)
     {
       outputStream_ << 
         setw(FLOAT_SETW) << it->x() << " " << 
         setw(FLOAT_SETW) << it->y() << endl;
     }    
   }
   virtual void doPolygonIndices() {
     outputStream_ << "\nPOLYGON INDICES DATA:\n" << endl;
     for(MD2Data::PolygonsIndicesConstItor it = md2Data_->beginPolygonsIndices();
       it != md2Data_->endPolygonsIndices();
       ++it)
     {
       outputStream_ << "V: " << 
         setw(INT_SETW) << it->indexToVerticesArray(0) << " " <<
         setw(INT_SETW) << it->indexToVerticesArray(1) << " " <<
         setw(INT_SETW) << it->indexToVerticesArray(2) << " " << endl;

       outputStream_ << "T: " << 
         setw(INT_SETW) << it->indexToTexCoordsArray(0) << " " <<
         setw(INT_SETW) << it->indexToTexCoordsArray(1) << " " <<
         setw(INT_SETW) << it->indexToTexCoordsArray(2) << " " << endl;
     }    
   }
};

string&
toLower(string* str)
{
  for(int i=0; i<str->size(); ++i)
    (*str)[i] = tolower( (*str)[i] );
  return *str;
}

void
traverseDirectories(const string&    rootDir,
                    TranslatorRefPtr translator)
{
  DriveElementsEnumeratorWin32 elementsEnumerator(rootDir);

  typedef DriveElementsEnumeratorWin32::ItorAutoPtr ElemsItor;

  for(ElemsItor elemItor(elementsEnumerator.getItor());
    elemItor->isValid();
    elemItor->moveToNextElement())
  {
    if(elemItor->isDirectory()) {
      if( elemItor->getElementName() == "." ||
          elemItor->getElementName() == "..")
      {
        continue;
      }
      else
        traverseDirectories(
          rootDir + "\\" + elemItor->getElementName(),
          translator);        
    }
    else 
    {
      string FILE_NAME = elemItor->getElementName();
      ::toLower( &FILE_NAME );

      if( FILE_NAME.find(".md2") != string::npos )
      {
        try 
        {
          static SymbolTST<char> fileNames;
          static vector<int>     ambiguousFileNameCounter;

          if( fileNames.isIdMapped(FILE_NAME) )
            ++ambiguousFileNameCounter[ fileNames.getHandle(FILE_NAME) ];
          else
            ambiguousFileNameCounter.push_back(1);

          strstream fileValue;
          fileValue << 
            ambiguousFileNameCounter[ fileNames.getHandle(FILE_NAME) ] <<
            '\0';

          size_t extensionOffset = FILE_NAME.find(".md2");
          string outputFile = FILE_NAME.substr(0, extensionOffset) +
            fileValue.str() +
            + ".sh2";

          cout << "Converting: " + rootDir + "\\" + FILE_NAME +
            " -> " + outputFile;
      
          const size_t FROM_STR_BEGIN = 0;
          translator->execute(
            MD2Data(rootDir + "\\" + FILE_NAME),
            outputFile);

          cout << " DONE!" << endl;
        }
        catch( Exception& except ) {
          cout << " FAIL!" << "\nException description:\n  " << 
            except.what() << endl;
        }
      } 
    } // end of if element not directory
  } // end of loop
}

void
usePatternPrint()
{
  cout << 
    "Example:\n"
    "  MD2toSH2 root_of_MD2_file_search_directory\n"
    "    One argument force an binary output\n"
    "  MD2toSH2 root_of_MD2_file_search_directory --plain-text\n"
    "    In this case output format will be plain text\n" << endl;
  cout <<
    "Program description:\n"
    "  This small program is used to translate Quake2 binary data\n"
    "format that is particular to Win32 platform. Programm execution\n"
    "yields an usefull output data - prebuilded binary data, or plain text.\n"
    "Especially that second format is important, becouse data based on\n"
    "a text format is cross-platform.\n"
    "  In addition programm execution do a specyfic work, that compute\n"
    "vertices normals, and decompress vertexes data.\n" << endl;  
  cout <<
    "Values that describes size of below enumerated types:\n"
    "  sizeof(int)   = " << sizeof(int) << endl <<
    "  sizeof(float) = " << sizeof(float) << endl << endl;
  cout <<
    "Data layout of sh2 Windows binary format:\n"
    "*int\n"
    "  frames count\n"
    "*int\n"
    "  vertices number per frame\n"
    "*int\n"
    "  polygons count\n"
    "*int\n"
    "  texture coordinates count\n"
    "*3x float (one vertice) * vertices per frame * frames count\n"
    "  vertices data for all key frames\n"
    "*3x float (one vertice) * vertices per frame * frames count\n"
    "  normals assigned to vertices for all key frames\n"
    "*6x ushort (3x indexes to vertices array and 3x indexes to textures\n"
    " coordinates array) * polygons count\n"
    "  geometry object construct data\n" 
    "*2x float (texture coordinate) * vertices per frame\n"
    "  texture coordinates assigned to vertices" << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 2) {
    usePatternPrint();
    return EXIT_SUCCESS;
  }
  else {
    string rootDirectory(argv[1]);
    TranslatorRefPtr translator;
    if(argc == 3 && (string(argv[2]).compare("--plain-text") == 0) )
      translator = new MD2DataConverterToPlainText;
    else
      translator = new MD2DataConverterToSH2Format;
    traverseDirectories(rootDirectory, translator);
  }
  return EXIT_SUCCESS;
}

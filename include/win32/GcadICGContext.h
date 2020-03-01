#ifndef _GCAD_ICGCONTEXT_H_
#define _GCAD_ICGCONTEXT_H_

#include <cg/cg.h>
#include <string>

namespace Gcad {
namespace RenderSystem {

class ICGShaderProgram;

/**
  @brief
    Opakowanie tzw. kontekstu wystepujacego w bibliotece Cg.
    Instancje typu odpowiadaja za wczytanie, oraz zarzadzanie
    programami shaderow
*/
class ICGContext {
 public:
   /**
     @brief
       Profile wyznaczajace zbior wlasciwosci wykorzystywanych podczas 
       kompilowania danej jednostki zawierajacej kod zrodlowy shadera
   */
   enum Profile {
     VS_1_1, VS_2_0, VS_2_X,
     PS_1_1, PS_1_2, PS_1_3, PS_2_0, PS_2_X
   };

   /**
     @brief
       Klasa podstawowa musi byc zawsze wyposazona w konstruktor wirtualny
   */
   virtual ~ICGContext() = 0;
   
   /**
     @brief
       Metoda wczytuje, oraz zapamietuje program shadera w wewnetrznej
       strukturze zarzadzajacej

     @param
       programId Jednoznaczny identyfikator okreslajacy program shadera

     @param 
       fileName Nazwa pliku zawierajacego program shadera

     @param
       entryPoint Nazwa funkcji wejscia shadera

     @param
       profile Profil wykorzystywany podczas kompilacji shadera
   */
   virtual void 
     createProgramFromSrcFile( const std::string&  programId,
                               const std::string&  fileName,
                               const std::string&  entryPoint,
                               Profile             profile ) = 0;

   /**
     @brief
       Pozyskanie shadera okreslonego przez argument, przechowywanego 
       w wewnetrznej strukturze
   */
   virtual ICGShaderProgram&  
     aquireProgram( const std::string& programId ) = 0;

   /**
     @brief
       Usuniecie z wewnetrznej struktury shadera o zadanym identyfikatorze
   */
   virtual void
     removeProgram( const std::string& programId ) = 0;
};

} // namespace RenderSystem
} // namespace Gcad

#endif
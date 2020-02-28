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

#ifndef _GCAD_CGFXMANAGER_H_
#define _GCAD_CGFXMANAGER_H_

#include "GcadBase.h"
#include "GcadException.h"
#include "GcadSingleton.h"
#include "GcadAssociativeManager.h"
#include <CgFX/ICgFXEffect.h>
#include <string>

namespace Gcad {
namespace RenderSystem {

/**
  @brief
    Klasa opakowujaca funkcjonalnosc zwiazana z podstawowymi
    konstrukcjami zawartymi w bibliotece CgFX, 
    oraz zarzadzajaca obiektami efektow, utworzonymi z udzialem
    funkcji interfejsu biblioteki CgFX
*/
class GCAD_EXPORT CgFxManager : public Utilities::Singleton<CgFxManager> {
 public:
   typedef Utilities::AssociativeManager<ICgFXEffect> 
     Effects;
   
   typedef Utilities::AssociativeManager<ICgFXEffectCompiler> 
     EffectCompilers;
   
   /**
     @brief
       Obiekt wyjatku powstalego w czasie proby utworzenia 
       instancji efektu zakonczonej niepowodzeniem
   */
   class GCAD_EXPORT EffectCreationFailed : public Utilities::Exception {
    private:
      friend class CgFxManager;
      
      EffectCreationFailed(const char* errorDescription) 
        : Utilities::Exception(errorDescription)
      {
      }
   };

   ICgFXEffect* 
     createEffectFromString(const std::string& effectId,
                            const char* srcData,
                            const char* compilerArgs = "");
   ICgFXEffect* 
     createEffectFromFile(const std::string& effectId,
                          const char* srcFile,
                          const char* compilerArgs = "");
   ICgFXEffectCompiler*
     createEffectCompilerFromString(const std::string& effectId,
                                    const char* srcData, 
                                    const char* compilerArgs = "");
   ICgFXEffectCompiler*
     createEffectCompilerFromFile(const std::string& effectId,
                                  const char* srcFile,
                                  const char* compilerArgs = "");

   ICgFXEffect* 
     getEffect(const std::string& effectId);

   ICgFXEffectCompiler* 
     getEffectCompiler(const std::string& effectId);
  
   void removeEffect(const std::string& effectId);
   void removeEffectCompiler(const std::string& effectId);

   void removeAllEffects();
   void removeAllEffectCompilers();

   /**
     @brief
       Wykonanie operacji OnLostDevice na wszystkich elementach
       znajdujacych sie w agregacie
   */
   void onLostDevice();

   /**
     @brief
       Wykonanie operacji OnResetDevice na wszystkich elementach
       znajdujacych sie w agregacie
   */
   void onResetDevice();

 private:
   Effects          effectManager_;
   EffectCompilers  effectCompilerManager_;
};

} // namespace RenderSystem
} // namespace Gcad

#endif

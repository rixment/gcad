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

#include "GcadCgFxManager.h"
#include "GcadAssertion.h"

namespace {

using namespace Gcad::Utilities;

/**
  @brief
    Szablon funkcji wykonujacy operacje, zdefiniowana przez drugi
    argument wywolania, na kazdym obiekcie efektu znajdujacym sie 
    w menadzerze zasobow okreslonym przez pierwszy argument wywolania
*/
template< typename RESOURCE, 
          typename EFFECT_INTERFACE >
void
executeForAllEffects( AssociativeManager<RESOURCE>& manager,
                      HRESULT (EFFECT_INTERFACE::*ptr)() )
{
  typedef AssociativeManager<RESOURCE>    Manager;
  typedef typename Manager::IDsConstItor  ManagerItor;
  typedef typename Manager::KeyType       ManagerKeyType;
  typedef typename Manager::KeyType       ManagerKeyType;

  for(ManagerItor curr = manager.begin();
    curr != manager.end();
    ++curr)
  {
    const ManagerKeyType& programId = *curr;
    EFFECT_INTERFACE& effect = manager.dereference(programId);
    ( effect.*ptr )();
  }
}

} // anonymous

namespace Gcad {
namespace RenderSystem {

ICgFXEffect* 
CgFxManager
::createEffectFromString(const std::string& effectId,
                         const char* srcData,
                         const char* compilerArgs)
{
  Utilities::assertion(srcData != 0, "Bledny wskaznik!");

  ICgFXEffect* effect = 0;
  const char*  errors = 0;
  
  if( FAILED(CgFXCreateEffect(srcData, &compilerArgs, &effect, &errors)) ) 
  {
    throw EffectCreationFailed(errors);
  }

  effectManager_.addResource(effectId, effect);
  
  return effect;
}
 
ICgFXEffect* 
CgFxManager
::createEffectFromFile(const std::string& effectId,
                       const char* srcFile,
                       const char* compilerArgs)
{
  Utilities::assertion(srcFile != 0, "Bledny wskaznik!");

  ICgFXEffect* effect = 0;
  const char*  errors = 0;
  
  if( FAILED( 
    CgFXCreateEffectFromFileA(srcFile, &compilerArgs, &effect, &errors) ) ) 
  {
    throw EffectCreationFailed(errors);
  }

  effectManager_.addResource(effectId, effect);
  
  return effect;
}
   
ICgFXEffectCompiler*
CgFxManager
::createEffectCompilerFromString(const std::string& effectId,
                                 const char* srcData, 
                                 const char* compilerArgs)
{
  Utilities::assertion(srcData != 0, "Bledny wskaznik!");

  ICgFXEffectCompiler* effect = 0;
  const char*          errors = 0;
 
  if( FAILED(
    CgFXCreateEffectCompiler(srcData, &compilerArgs, &effect, &errors) ) )
  {
    throw EffectCreationFailed(errors);
  }

  effectCompilerManager_.addResource(effectId, effect);

  return effect;
}
 
ICgFXEffectCompiler*
CgFxManager
::createEffectCompilerFromFile(const std::string& effectId,
                               const char* srcFile,
                               const char* compilerArgs)
{
  Utilities::assertion(srcFile != 0, "Bledny wskaznik!");

  ICgFXEffectCompiler* effect = 0;
  const char*          errors = 0;
 
  if( FAILED(CgFXCreateEffectCompilerFromFileA(srcFile, 
    &compilerArgs, &effect, &errors)) )
  {
    throw EffectCreationFailed(errors);
  }

  effectCompilerManager_.addResource(effectId, effect);

  return effect;
}

ICgFXEffect* 
CgFxManager
::getEffect(const std::string& effectId)
{
  return &effectManager_.dereference(effectId);
}

ICgFXEffectCompiler* 
CgFxManager
::getEffectCompiler(const std::string& effectId)
{
  return &effectCompilerManager_.dereference(effectId);
}

void 
CgFxManager
::removeEffect(const std::string& effectId)
{
  effectManager_.removeResource(effectId);
}

void 
CgFxManager
::removeEffectCompiler(const std::string& effectId)
{
  effectCompilerManager_.removeResource(effectId);
}
   
void 
CgFxManager
::removeAllEffects()
{
  effectManager_.removeAll();
}

void 
CgFxManager
::removeAllEffectCompilers()
{
  effectCompilerManager_.removeAll();
}

void
CgFxManager
::onLostDevice()
{
  executeForAllEffects( effectManager_, &ICgFXEffect::OnLostDevice );
}

void
CgFxManager
::onResetDevice()
{
  executeForAllEffects( effectManager_, &ICgFXEffect::OnResetDevice );
}

} // namespace RenderSystem
} // namespace Gcad
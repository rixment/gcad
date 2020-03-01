#include "GcadCGContextD3D9.h"
#include "GcadCGShaderProgramD3D9.h"
#include "GCadComUtilities.h"
#include "GCadAssertion.h"

#include <cg/cgD3D9.h>

namespace {

using namespace Gcad::RenderSystem;
using namespace Gcad::Utilities;

/**
  @brief
    Funkcja wykonujaca odwzorowanie profilu
*/
CGprofile
determineD3D9Profile(ICGContext::Profile profile)
{
  switch(profile) {
    case ICGContext::VS_1_1: return CG_PROFILE_VS_1_1;
    case ICGContext::VS_2_0: return CG_PROFILE_VS_2_0;
    case ICGContext::VS_2_X: return CG_PROFILE_VS_2_X;
    case ICGContext::PS_1_1: return CG_PROFILE_PS_1_1;
    case ICGContext::PS_1_2: return CG_PROFILE_PS_1_2;
    case ICGContext::PS_1_3: return CG_PROFILE_PS_1_3;
    case ICGContext::PS_2_0: return CG_PROFILE_PS_2_0;
    case ICGContext::PS_2_X: return CG_PROFILE_PS_2_X;
  }
  assertion( false, "Blad wewnetrzny! Przekazano nieprawidlowy profil!" );
}

} // anonymous namespace

namespace Gcad {
namespace RenderSystem {

CGContextD3D9
::CGContextD3D9( IDirect3DDevice9* device ) 
{
  Utilities::assignCOMInterface( device, &device_ );
  cgContext_ = cgCreateContext();
}
   
CGContextD3D9
::~CGContextD3D9() 
{
  cgDestroyContext( cgContext_ );
  Utilities::releaseCOMInterface( &device_ );
}

void
CGContextD3D9
::createProgramFromSrcFile( const std::string&  programId,
                            const std::string&  fileName,
                            const std::string&  entryPoint,
                            Profile             profile )
{
  CGprofile cgProfile = determineD3D9Profile(profile);
  
  ICGShaderProgram* shader = new CGShaderProgramD3D9( 
    cgCreateProgramFromFile( cgContext_,
                             CG_SOURCE,
                             fileName.c_str(),
                             cgProfile,
                             entryPoint.c_str(),
                             0 ) );

  shadersManager_.addResource( programId, shader );
}

ICGShaderProgram&  
CGContextD3D9
::aquireProgram( const std::string& programId )
{
  return shadersManager_.dereference(programId);
}

void
CGContextD3D9
::removeProgram( const std::string& programId )
{
  shadersManager_.removeResource(programId);
}

}
}
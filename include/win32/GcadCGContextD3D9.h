#ifndef _GCAD_CGCONTEXTD3D9_H_
#define _GCAD_CGCONTEXTD3D9_H_

#include "GcadICGContext.h"
#include "GcadAssociativeManager.h"
#include <d3d9.h>
#include <cg/cg.h>

namespace Gcad {
namespace RenderSystem {

/**
  @brief
    Implementacja interfejsu kontekstu Cg wykonana dla biblioteki
    DirectX9
*/
class CGContextD3D9 : public ICGContext {
 public:
   typedef Utilities::AssociativeManager< ICGShaderProgram >  Shaders;

   CGContextD3D9( IDirect3DDevice9* device );
   
   virtual ~CGContextD3D9();

   virtual void 
     createProgramFromSrcFile( const std::string&  programId,
                               const std::string&  fileName,
                               const std::string&  entryPoint,
                               Profile             profile );

   virtual ICGShaderProgram&  
     aquireProgram( const std::string& programId );

   virtual void
     removeProgram( const std::string& programId );

 private:
   IDirect3DDevice9*  device_;
   CGcontext          cgContext_;
   Shaders            shadersManager_;  
};

} // namespace RenderSystem
} // namespace Gcad

#endif
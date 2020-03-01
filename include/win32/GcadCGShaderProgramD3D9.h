#ifndef _GCAD_CGSHADERPROGRAMD3D9_H_
#define _GCAD_CGSHADERPROGRAMD3D9_H_

#include "GcadICGShaderProgram.h"
#include <cg/cg.h>

namespace Gcad {
namespace RenderSystem {

class CGShaderProgramD3D9 : public ICGShaderProgram {
 public:
   CGShaderProgramD3D9( CGprogram program );

   virtual ~CGShaderProgramD3D9();

   virtual void load();

   virtual void bind();

   virtual void setParameterFloat( const std::string&  name,
                                   float               value );

 private:
   CGprogram cgProgram_;
};

} // namespace RenderSystem
} // namespace Gcad

#endif
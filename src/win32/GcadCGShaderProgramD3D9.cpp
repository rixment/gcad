#include "GCadCGShaderProgramD3D9.h"
#include <cg/cgD3D9.h>

namespace Gcad {
namespace RenderSystem {

CGShaderProgramD3D9
::CGShaderProgramD3D9( CGprogram program )
  : cgProgram_( program )
{
}

CGShaderProgramD3D9
::~CGShaderProgramD3D9()
{
  cgDestroyProgram( cgProgram_ );
}

void 
CGShaderProgramD3D9
::load()
{
  cgD3D9LoadProgram( cgProgram_, CG_FALSE, 0 );
}

void 
CGShaderProgramD3D9
::bind()
{
  cgD3D9BindProgram( cgProgram_ );
}

void 
CGShaderProgramD3D9
::setParameterFloat( const std::string&  name,
                     float               value )
{
  CGparameter param = cgGetNamedParameter( cgProgram_, name.c_str() );
  cgD3D9SetUniformArray(param, 0, 1, &value);
}

} // namespace RenderSystem
} // namespace Gcad
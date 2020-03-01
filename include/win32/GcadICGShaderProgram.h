#ifndef _GCAD_ICGSHADERPROGRAM_H_
#define _GCAD_ICGSHADERPROGRAM_H_

#include <string>

namespace Gcad {
namespace RenderSystem {

class ICGShaderProgram {
 public:
   virtual ~ICGShaderProgram() = 0;

   virtual void load() = 0;

   virtual void bind() = 0;

   virtual void setParameterFloat( const std::string&  name,
                                   float               value ) = 0;
};

} // namespace RenderSystem
} // namespace Gcad

#endif
#ifndef __Shader3D_H__
#define __Shader3D_H__

#include <stdint.h>

namespace laya
{

class Shader3D
{
  public:
    static uint32_t CULL;
    static uint32_t BLEND;
    static uint32_t BLEND_SRC;
    static uint32_t BLEND_DST;
    static uint32_t BLEND_SRC_RGB;
    static uint32_t BLEND_DST_RGB;
    static uint32_t BLEND_SRC_ALPHA;
    static uint32_t BLEND_DST_ALPHA;
    static uint32_t BLEND_EQUATION;
    static uint32_t BLEND_EQUATION_RGB;
    static uint32_t BLEND_EQUATION_ALPHA;
    static uint32_t DEPTH_TEST;
    static uint32_t DEPTH_WRITE;
    static uint32_t STENCIL_TEST;
    static uint32_t STENCIL_WRITE;
    static uint32_t STENCIL_Ref;
    static uint32_t STENCIL_Op;
    static void __init__();
};
} // namespace laya

#endif //__Shader3D_H__

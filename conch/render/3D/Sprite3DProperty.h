#ifndef __Sprite3DProperty_H__
#define __Sprite3DProperty_H__

#include "render/driver/gles/LayaGL.h"

namespace laya
{
class Sprite3DProperty
{
  public:
    static uint32_t WORLDMATRIX;
    static uint32_t WORLDINVERTFRONT;
    static void __init__();
};

} // namespace laya
#endif
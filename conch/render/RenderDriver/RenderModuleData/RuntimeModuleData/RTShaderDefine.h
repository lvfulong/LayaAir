#ifndef __ShaderDefine_H__
#define __ShaderDefine_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <stdio.h>
#include <unordered_map>

namespace laya
{
class ShaderDefine
{
    // static _texGammaDefine : {[key:string] : ShaderDefine} = {}; todo
  public:
    int32_t _index;
    int32_t _value;

    ShaderDefine(int32_t index, int32_t value)
    {
        this->_index = index;
        this->_value = value;
    }
};
} // namespace laya
#endif

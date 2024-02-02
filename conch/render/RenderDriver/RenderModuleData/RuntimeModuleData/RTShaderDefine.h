#ifndef __ShaderDefine_H__
#define __ShaderDefine_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <stdio.h>
#include <unordered_map>
#include <functional>
#include <utils/hash.h>

namespace laya
{
class ShaderDefine
{
    // static _texGammaDefine : {[key:string] : ShaderDefine} = {}; todo
  public:
    int32_t _index;
    int32_t _value;

    ShaderDefine(int32_t index = -1, int32_t value = -1)
    {
        this->_index = index;
        this->_value = value;
    }
    bool operator==(const ShaderDefine& rhs) const
    {
        return (this->_index == rhs._index) && (this->_value == rhs._value);
    }
};


} // namespace laya
namespace std
{
template <>
struct hash<laya::ShaderDefine>
{
    size_t operator()(const laya::ShaderDefine &rhs) const
    {
        size_t seed = 0;
        laya::hash_combine(seed, std::hash<int>{}(rhs._index));
        laya::hash_combine(seed, std::hash<int>{}(rhs._value));
        return seed;
   }
};
} // namespace std
#endif

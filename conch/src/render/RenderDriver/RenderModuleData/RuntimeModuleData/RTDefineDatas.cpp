#include "RTDefineDatas.h"
#include <algorithm>
#include <utils/Log.h>

namespace laya
{

RTDefineDatas::RTDefineDatas()
{

}

RTDefineDatas::~RTDefineDatas()
{

    _mask.clear();
}

void RTDefineDatas::_intersectionDefineDatas(RTDefineDatas *define)
{
    for (int32_t i = _length - 1; i >= 0; i--)
    {
        int64_t value = 0;
        //神坑 JS与C++差别导致
        if (i < define->_mask.size())
            value = _mask[i] & define->_mask[i];
        else
            value = _mask[i] & 0;

        if (value == 0 && i == (_length - 1))
        {
            _length--;

        }
        else
            _mask[i] = value;
    }
}

void RTDefineDatas::add(RTShaderDefine define)
{
    int32_t index = define._index;
    int32_t size = index + 1;

    int32_t maskStart = this->_length;
    // must from this._length because this._length maybe less than mask.length and have dirty data should clear.
    if (maskStart < size)
    {
        if (_mask.size() < size)
            (_mask.resize(size)); // mask.length maybe small than size,maybe not.
        for (; maskStart < index; maskStart++)
            _mask[maskStart] = 0;
        _mask[index] = define._value;
        _length = size;
    }
    else
    {
        _mask[index] |= define._value;
    }
}

void RTDefineDatas::remove(RTShaderDefine define)
{
    int32_t index = define._index;
    // var mask : Array<number> = this._mask;
    int32_t endIndex = _length - 1;
    if (index > endIndex) // ������Length,���⾭������
        return;
    int64_t newValue = _mask[index] & ~define._value;
    if (index == endIndex && newValue == 0)
        _length--;
    else
        _mask[index] = newValue;
}

bool RTDefineDatas::has(RTShaderDefine define)
{
    int32_t index = define._index;
    if (index >= _length)
        return false;
    return ((_mask[index] & define._value) != 0);
}

void RTDefineDatas::clear()
{
    _length = 0;
    _mask.resize(0);
}

void RTDefineDatas::addDefineDatas(RTDefineDatas *defines)
{
    // var addMask : Array<number> = define._mask;
    int32_t size = defines->_length;
    // var mask : Array<number> = this._mask;
    int32_t maskStart = _length;
    if (maskStart < size)
    {
        _mask.resize(size);
        int32_t i = 0;
        for (; i < maskStart; i++)
            _mask[i] |= defines->_mask[i];
        for (; i < size; i++)
            _mask[i] = defines->_mask[i]; 
        _length = size;
    }
    else
    {
        for (uint32_t i = 0; i < size; i++)
        {
            _mask[i] |= defines->_mask[i];
        }
    }
}

void RTDefineDatas::removeDefineDatas(RTDefineDatas *defines)
{
    // var removeMask : Array<number> = define._mask;
    // var mask : Array<number> = this._mask;
    int32_t endIndex = _length - 1;
    int32_t i = std::min(defines->_length, endIndex);
    for (; i >= 0; i--)
    {
        int64_t newValue = _mask[i] & ~defines->_mask[i];
        if (i == endIndex && newValue == 0)
        {
            endIndex--;
            _length--;
        }
        else
        {
            _mask[i] = newValue;
        }
    }
}

void RTDefineDatas::cloneTo(RTDefineDatas *defines)
{
    /*	var destDefineData : DefineDatas = (<DefineDatas>destObject);
        var destMask : Array<number> = destDefineData._mask;
        var mask : Array<number> = this._mask;*/
    int32_t count = _length;
    defines->_mask.resize(count);
    for (uint32_t i = 0; i < count; i++)
        defines->_mask[i] = _mask[i];
    defines->_length = _length;
}
void RTDefineDatas::destroy()
{
    // TODO
}
} // namespace laya
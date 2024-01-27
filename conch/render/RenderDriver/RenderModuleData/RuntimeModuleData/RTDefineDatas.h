#ifndef __DefineDatas_H__
#define __DefineDatas_H__
#include <vector>
#include "RTShaderDefine.h"
namespace laya{

    class DefineDatas {
    public:
        DefineDatas();
        ~DefineDatas();
        void _intersectionDefineDatas(DefineDatas* define);
        void add(ShaderDefine* define);
        void remove(ShaderDefine* define);
        bool has(ShaderDefine* define);
        void clear();
        void addDefineDatas(DefineDatas* defines);
        void removeDefineDatas(DefineDatas* defines);
        void cloneTo(DefineDatas* defines);
        DefineDatas* clone();
        void destroy();
    public:
        std::vector<uint32_t> _mask{};
        uint32_t _length = 0;
    };
}

#endif//__DefineDatas_H__
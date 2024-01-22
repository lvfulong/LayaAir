#ifndef __SubShader_H__
#define __SubShader_H__

#include <vector>
#include "ShaderPass.h"

namespace laya {

    class SubShader {
    public:
        SubShader();
        ~SubShader();
        void addShaderpass(ShaderPass* pass);
    public:
        std::vector<ShaderPass*> shaderpasses;
    };
}

#endif//__SubShader_H__
#ifndef __JSSubtleCrypto_H__
#define __JSSubtleCrypto_H__

#include <jsbind/JSBind.h>

namespace laya 
{

    class JSSubtleCrypto
    {
    public:
        static void exportJS(jsbind::Object& context);
    public:
        static jsvm_value digest(const char* algorithm, const jsbind::ArrayBuffer& data);
    };

}
#endif 

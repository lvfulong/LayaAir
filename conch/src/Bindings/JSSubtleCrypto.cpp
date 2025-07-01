#include "JSSubtleCrypto.h"
#include "../../JCConch.h"
#include <crypto_core/CryptoCore.h>
#include <profiler/Profiler.h>
namespace laya 
{
    jsvm_value JSSubtleCrypto::digest(const char* algorithm, const jsbind::ArrayBuffer& data) 
    {
        Profiler_ZoneScoped("JSSubtleCrypto::digest", 0xff0000);
        if (strcmp(algorithm, "SHA-1") == 0)    
        {
            static unsigned char hash[SHA1_DIGEST_LENGTH];
            if (!CryptoCore::sha1(data.getData(), data.getByteLength(), hash))
            {
                return jsbind::makeNull();
            }
            jsbind::ArrayBuffer result = jsbind::ArrayBuffer::MakeArrayBuffer(hash, SHA1_DIGEST_LENGTH);
            return result.getHandle();      
        }
        return jsbind::makeNull();
    }
    void JSSubtleCrypto::exportJS(jsbind::Object& context)
    {
        jsbind::global_class_<JSSubtleCrypto> class_binding;
		class_binding.class_function("digest", &JSSubtleCrypto::digest);
        context.global_class_("conchSubtleCrypto", class_binding);
    }
}

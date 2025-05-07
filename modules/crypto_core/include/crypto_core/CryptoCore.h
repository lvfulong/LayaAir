#ifndef __CRYPTO_H__  
#define __CRYPTO_H__

#include <cstddef>

namespace laya
{
    constexpr size_t SHA1_DIGEST_LENGTH = 20;
    class CryptoCore
    {
    public:
        static  bool sha1(const unsigned char* source, size_t length, unsigned char hash[SHA1_DIGEST_LENGTH]);
    };  
} // namespace laya
#endif
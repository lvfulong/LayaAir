#include <crypto_core/CryptoCore.h>
#include <utils/Log.h>
#include <mbedtls/sha1.h>
#include <cstring>

namespace laya
{
    bool CryptoCore::sha1(const unsigned char *source, size_t length, unsigned char hash[SHA1_DIGEST_LENGTH])
    {
        mbedtls_sha1_context ctx;
        int ret;
        memset(&ctx, 0, sizeof(ctx));
        mbedtls_sha1_init(&ctx);
        ret = mbedtls_sha1_starts(&ctx);
        if (ret != 0)
        {
            return false;
        }
        ret = mbedtls_sha1_update(&ctx, source, length);
        if (ret != 0)
        {
            return false;
        }
        ret = mbedtls_sha1_finish(&ctx, hash);
        if (ret != 0)
        {
            return false;
        }
        mbedtls_sha1_free(&ctx);
        return true;
    }
} // namespace laya

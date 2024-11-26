#ifndef KTXTextureInfo_h
#define KTXTextureInfo_h

#include <jsbind/JSBind.h>
#include <render/3D/design/renderEnum/TextureDimension.h>
#include <render/3D/design/renderEnum/TextureFormat.h>

namespace laya
{

struct KTXTextureInfo
{
    const char *source;
    bool compress;
    bool sRGB;
    TextureDimension dimension;
    int width;
    int height;
    TextureFormat format;
    int mipmapCount;
    int bytesOfKeyValueData;
    int headerOffset;
};
struct KTXTextureInfoJS
{
    jsbind::ArrayBuffer sourceAB;
    bool compress;
    bool sRGB;
    TextureDimension dimension;
    int width;
    int height;
    TextureFormat format;
    int mipmapCount;
    int bytesOfKeyValueData;
    int headerOffset;
};

struct DDSTextureInfo
{
    const char *source;
    bool compressed;
    int dataOffset;
    int blockBytes;
    int mipmapCount;
    int bpp;
    bool isCube;
    int width;
    int height;
    TextureFormat format;
};
struct DDSTextureInfoJS
{
    jsbind::ArrayBuffer sourceAB;
    bool compressed;
    int dataOffset;
    int blockBytes;
    int mipmapCount;
    int bpp;
    bool isCube;
    int width;
    int height;
    TextureFormat format;
};
struct FormatParam
{
    int channels;
    int bytesPerPixel;
    int typesSize;
};
} // namespace laya

#endif /* KTXTextureInfo_h */

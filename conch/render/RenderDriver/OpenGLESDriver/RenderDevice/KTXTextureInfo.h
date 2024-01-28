#ifndef KTXTextureInfo_h
#define KTXTextureInfo_h

#include <binder/JSInterface.h>
#include <render/3D/design/renderEnum/TextureDimension.h>
#include <render/3D/design/renderEnum/TextureFormat.h>

namespace laya
{

struct KTXTextureInfo
{
    char *source;
    bool compress;
    bool sRGB;
    TextureDimension dimension;
    int width;
    int height;
    TextureFormat format;
    int mipmapCount;
    int bytesOfKeyValueData;
    int headerOffset;

    //static KTXTextureInfo parseJS(JSValueAsParam jsKTXInfo);
};
struct DDSTextureInfo
{
    char *source;
    bool compressed;
    int dataOffset;
    int blockBytes;
    int mipmapCount;
    int bbp;
    TextureFormat format;

    //static DDSTextureInfo parseJS(JSValueAsParam jsDDSInfo);
};

struct FormatParam
{
    int channels;
    int bytesPerPixel;
    int typesSize;
};
} // namespace laya

#endif /* KTXTextureInfo_h */

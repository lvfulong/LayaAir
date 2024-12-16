#ifndef __GLTextureContext_H__
#define __GLTextureContext_H__

#include "KTXTextureInfo.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLCapable.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include <array>
#include <memory>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <render/3D/design/renderEnum/RenderTargetFormat.h>
#include <render/3D/design/renderEnum/TextureCompareMode.h>
#include <render/3D/design/renderEnum/TextureDimension.h>
#include <render/3D/design/renderEnum/TextureFormat.h>
#include <string>
#include <unordered_map>
#include <utils/Preprocessor.h>

namespace laya
{
struct GLParam
{
    int internalFormat = 0;
    int format = 0;
    int type = 0;
};
struct GLRenderBufferParam
{
    int internalFormat;
    int attachment;
};
struct FormatPixelsParams
{
    int channels = 0;
    int bytesPerPixel = 0;
    // dataTypedCons: Uint8Array,
    int typedSize = 1;
};
class JSImage;
class InternalTexture;
class JCImage;
class GLESInternalTex;
class GLESInternalRT;
class GLTextureContext : public GLObject
{
  public:
    GLTextureContext();
    ~GLTextureContext();
    FormatPixelsParams &getFormatPixelsParams(TextureFormat format, FormatPixelsParams &formatParams);
    virtual const GLParam &glTextureParam(TextureFormat format, bool useSRGB);
    virtual const GLParam &glRenderTextureParam(RenderTargetFormat format, bool useSRGB);
    virtual GLRenderBufferParam *glRenderBufferParam(RenderTargetFormat format, bool useSRGB);
    virtual GLenum glRenderTargetAttachment(RenderTargetFormat format);
    virtual GLenum getTarget(TextureDimension dimension);
    virtual bool supportSRGB(int format /*TextureFormat | RenderTargetFormat*/, bool mipmap);
    virtual bool supportGenerateMipmap(int format /*TextureFormat | RenderTargetFormat*/);
    virtual bool isSRGBFormat(int format /*TextureFormat | RenderTargetFormat*/);
    virtual GLESInternalTex *createTextureInternal(TextureDimension dimension, int width, int height,
                                                   TextureFormat format, bool gengerateMipmap, bool sRGB,
                                                   bool premultiplyAlpha);
    virtual void setTextureImageData(GLESInternalTex *texture,
                                     JCImage *source /*HTMLImageElement | HTMLCanvasElement | ImageBitmap*/,
                                     bool premultiplyAlpha, bool invertY);
    virtual void setTexturePixelsData(GLESInternalTex *texture, char *source, int sourceBytes, bool premultiplyAlpha,
                                      bool invertY);
    virtual GLESInternalTex* createTexture3DInternal(TextureDimension dimension, int width, int height, int depth, TextureFormat format, bool generateMipmap, bool sRGB, bool premultipliedAlpha)
    {
        return nullptr;
    }
    virtual void setTexture3DImageData(GLESInternalTex *texture, const std::vector<JSImage *> &sources, int depth,
                                       bool premultiplyAlpha, bool invertY)
    {
        assert(0);
    };
    virtual void setTexture3DPixelsData(GLESInternalTex *texture, char *source, int sourceBytes, int depth,
                                        bool premultiplyAlpha, bool invertY)
    {
        assert(0);
    };
    virtual void setTexture3DSubPixelsData(GLESInternalTex *texture, char *source, int mipmapLevel, bool generateMipmap,
                                           int xOffset, int yOffset, int zOffset, int width, int height, int depth,
                                           bool premultiplyAlpha, bool invertY)
    {
        assert(0);
    };
    virtual void initVideoTextureData(GLESInternalTex *texture);
    virtual void setTextureSubPixelsData(GLESInternalTex *texture, char *source, int mipmapLevel, bool generateMipmap,
                                         int xOffset, int yOffset, int width, int height, bool premultiplyAlpha,
                                         bool invertY);
    virtual void bindRenderTarget(GLESInternalRT *renderTarget, int faceIndex);
    virtual TextureCompareMode setTextureCompareMode(GLESInternalTex *texture, TextureCompareMode compareMode);
    virtual void setupRendertargetTextureAttachment(GLESInternalRT *renderTarget, GLESInternalTex *texture);
    virtual void bindoutScreenTarget();
    virtual void unbindRenderTarget(GLESInternalRT *renderTarget);
   
    virtual GLESInternalTex *createRenderTextureCubeInternal(TextureDimension dimension, int size,
                                                             RenderTargetFormat format, bool generateMipmap, bool sRGB);
    virtual GLuint createRenderbuffer(int width, int height, int internalFormat, int samples);
    virtual GLESInternalRT *createRenderTargetInternal(int width, int height, RenderTargetFormat colorFormat,
                                                       RenderTargetFormat depthStencilFormat, bool generateMipmap,
                                                       bool sRGB, int multiSamples);
    virtual GLESInternalRT *createRenderTargetCubeInternal(int size, RenderTargetFormat colorFormat,
                                                           RenderTargetFormat depthStencilFormat, bool generateMipmap,
                                                           bool sRGB, int multiSamples);
    virtual void setTextureDDSData(GLESInternalTex *texture, const DDSTextureInfo &ddsInfo);
    virtual void setTextureKTXData(GLESInternalTex *texture, const KTXTextureInfo &ktxInfo);
    virtual void setCubeImageData(GLESInternalTex *texture, const std::vector<JSImage *> &sources,
                                  bool premultiplyAlpha, bool invertY);
    virtual void setCubePixelsData(GLESInternalTex *texture, const std::vector<char *> &source, bool premultiplyAlpha,
                                   bool invertY);
    virtual void setCubeSubPixelData(GLESInternalTex *texture, const std::vector<char *> &source, int mipmapLevel,
                                     bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                     bool premultiplyAlpha, bool invertY);
    virtual void setCubeDDSData(GLESInternalTex *texture, const DDSTextureInfo &ddsInfo);
    virtual void setCubeKTXData(GLESInternalTex *texture, const KTXTextureInfo &ktxInfo);
    virtual void readRenderTargetPixelData(GLESInternalRT *renderTarget, int xOffset, int yOffset, int width,
                                           int height, std::vector<uint8_t> &out);
   
    virtual void updateVideoTexture(GLESInternalTex *texture, JCImage *video, bool premultiplyAlpha, bool invertY);

    void setTexturePixelsDataJS(GLESInternalTex *texture, jsbind::ArrayBuffer arrayBuffer, bool premultiplyAlpha, bool invertY);
    void setTextureSubPixelsDataJS(GLESInternalTex *texture, jsbind::ArrayBuffer source, int mipmapLevel,
                                   bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                   bool premultiplyAlpha, bool invertY);
    void setCubePixelsDataJS(GLESInternalTex *texture, std::vector<jsbind::ArrayBuffer> source, bool premultiplyAlpha, bool invertY);
    void setCubeSubPixelDataJS(GLESInternalTex *texture, std::vector<jsbind::ArrayBuffer> source, int mipmapLevel, bool generateMipmap,
                               int xOffset, int yOffset, int width, int height, bool premultiplyAlpha, bool invertY);
    virtual GLESInternalTex* createRenderTargetDepthTexture(GLESInternalRT* renderTarget, TextureDimension dimension, int width, int height);
  protected:
      GLESInternalRT* currentActiveRT = nullptr;
    virtual int getGLtexMemory(GLESInternalTex *tex, int depth = 1);
    virtual int getGLRTTexMemory(int width, int height, RenderTargetFormat colorFormat,
                                 RenderTargetFormat depthStencilFormat, bool generateMipmap, int multiSamples,
                                 bool cube);
    virtual GLESInternalTex* createRenderTextureInternal(TextureDimension dimension, int width, int height,
        RenderTargetFormat format, bool generateMipmap, bool sRGB);
    sRGBExt *m_sRGB;
    OESTextureHalfFloatExt *m_oesTextureHalfFloat;
    CompressdTextureS3tc_srgbExt *m_compressdTextureS3tc_srgb;
    CompressedTextureEtc1Ext *m_compressedTextureEtc1;
    CompressedTextureS3tcExt *m_compressedTextureS3tc;
    CompressedTextureETCExt *m_compressedTextureETC;
    CompressedTextureASTCExt *m_compressedTextureASTC;
    WebglDepthTextureExt *m_webgl_depth_texture;
    GLParam m_glParam;
};
} // namespace laya
#endif

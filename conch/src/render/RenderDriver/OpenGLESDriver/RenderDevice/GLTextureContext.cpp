#include "GLTextureContext.h"
#include "GLESInternalRT.h"
#include "GLESInternalTex.h"
#include "Image/JCImage.h"
#include "JCConch.h"
#include "LayaGL/JCLayaGL.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLUtils.h"
#include "render/LayaGL.h"
#include <Bindings/JSImage.h>
#include <assert.h>
#include <utils/Preprocessor.h>
#include <utils/Log.h>

namespace laya
{
static void TexImage2DSafe(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
                           GLint border, GLenum format, GLenum type, const void *pixels)
{
    unsigned bytesPerComponent, componentsPerPixel;
    if (pixels == nullptr &&
        JCLayaGL::computeFormatAndTypeParameters(format, type, &componentsPerPixel, &bytesPerComponent))
    {
        int size = componentsPerPixel * bytesPerComponent * width * height;
        uint8_t *zeroData = new uint8_t[size];
        memset(zeroData, 0, size);
        glTexImage2D(target, level, internalformat, width, height, border, format, type, zeroData);
        delete[] zeroData;
    }
    else
    {
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }
}
extern int g_nMainFrameBuffer;
GLTextureContext::GLTextureContext() : GLObject(LayaGL::m_pWebglEngine)
{
    m_sRGB = (sRGBExt *)m_engine->getExtension(WebGLExtension::EXT_sRGB);
    m_oesTextureHalfFloat = (OESTextureHalfFloatExt *)m_engine->getExtension(WebGLExtension::OES_texture_half_float);
    m_compressdTextureS3tc_srgb =
        (CompressdTextureS3tc_srgbExt *)m_engine->getExtension(WebGLExtension::WEBGL_compressed_texture_s3tc_srgb);
    m_compressedTextureEtc1 =
        (CompressedTextureEtc1Ext *)m_engine->getExtension(WebGLExtension::WEBGL_compressed_texture_etc1);
    m_compressedTextureS3tc =
        (CompressedTextureS3tcExt *)m_engine->getExtension(WebGLExtension::WEBGL_compressed_texture_s3tc);
    m_compressedTextureETC =
        (CompressedTextureETCExt *)m_engine->getExtension(WebGLExtension::WEBGL_compressed_texture_etc);
    m_compressedTextureASTC =
        (CompressedTextureASTCExt *)m_engine->getExtension(WebGLExtension::WEBGL_compressed_texture_astc);
    m_webgl_depth_texture = (WebglDepthTextureExt *)m_engine->getExtension(WebGLExtension::WEBGL_depth_texture);
}
GLTextureContext::~GLTextureContext()
{
}
FormatPixelsParams &GLTextureContext::getFormatPixelsParams(TextureFormat format, FormatPixelsParams &formatParams)
{
    switch (format)
    {
    case TextureFormat::R8G8B8A8:
        formatParams.channels = 4;
        formatParams.bytesPerPixel = 4;
        // formatParams.dataTypedCons = Uint8Array
        formatParams.typedSize = 1;
        return formatParams;
    case TextureFormat::R8G8B8:
        formatParams.channels = 3;
        formatParams.bytesPerPixel = 3;
        // formatParams.dataTypedCons = Uint8Array
        formatParams.typedSize = 1;
        return formatParams;
    case TextureFormat::R5G6B5:
        formatParams.channels = 3;
        formatParams.bytesPerPixel = 2;
        // formatParams.dataTypedCons = Uint16Array
        formatParams.typedSize = 2;
        return formatParams;
    case TextureFormat::R16G16B16:
        formatParams.channels = 3;
        formatParams.bytesPerPixel = 6;
        // formatParams.dataTypedCons = Uint16Array
        formatParams.typedSize = 2;
        return formatParams;
    case TextureFormat::R16G16B16A16:
        formatParams.channels = 4;
        formatParams.bytesPerPixel = 8;
        // formatParams.dataTypedCons = Uint16Array
        formatParams.typedSize = 2;
        return formatParams;
    case TextureFormat::R32G32B32:
        formatParams.channels = 3;
        formatParams.bytesPerPixel = 12;
        // formatParams.dataTypedCons = Float32Array
        formatParams.typedSize = 4;
        return formatParams;
    case TextureFormat::R32G32B32A32:
        formatParams.channels = 4;
        formatParams.bytesPerPixel = 16;
        // formatParams.dataTypedCons = Float32Array
        formatParams.typedSize = 4;
        return formatParams;
    default:
        return formatParams;
    }
}
const GLParam &GLTextureContext::glTextureParam(TextureFormat format, bool useSRGB)
{
    m_glParam.internalFormat = 0;
    m_glParam.format = 0;
    m_glParam.type = 0;
    switch (format)
    {
    case TextureFormat::R8G8B8:
        if (m_sRGB != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_sRGB->SRGB_EXT : GL_RGB;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::R8G8B8A8:
        if (m_sRGB != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_sRGB->SRGB_ALPHA_EXT : GL_RGBA;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::R5G6B5:
        m_glParam.internalFormat = GL_RGB;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_UNSIGNED_SHORT_5_6_5;
        break;
    case TextureFormat::R32G32B32A32:
        m_glParam.internalFormat = GL_RGBA;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_FLOAT;
        break;
    case TextureFormat::R32G32B32:
        m_glParam.internalFormat = GL_RGB;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_FLOAT;
        break;
    case TextureFormat::R16G16B16A16:
        if (m_oesTextureHalfFloat != nullptr)
        {
            m_glParam.internalFormat = GL_RGBA;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = m_oesTextureHalfFloat->HALF_FLOAT_OES;
        }
        break;
    case TextureFormat::R16G16B16:
        if (m_oesTextureHalfFloat != nullptr)
        {
            m_glParam.internalFormat = GL_RGB;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = m_oesTextureHalfFloat->HALF_FLOAT_OES;
        }
        break;
    case TextureFormat::DXT1:
        if (m_compressdTextureS3tc_srgb != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
                                               : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT1_EXT;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::DXT3:
        if (m_compressdTextureS3tc_srgb != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
                                               : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT3_EXT;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::DXT5:
        if (m_compressdTextureS3tc_srgb != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_compressdTextureS3tc_srgb->COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
                                               : m_compressedTextureS3tc->COMPRESSED_RGBA_S3TC_DXT5_EXT;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC1RGB:
        if (m_compressedTextureEtc1 != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureEtc1->COMPRESSED_RGB_ETC1_WEBGL;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2RGBA:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_RGBA8_ETC2_EAC;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2RGB:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_RGB8_ETC2;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2SRGB:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_SRGB8_ETC2;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2SRGB_Alpha8:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_SRGB8_ALPHA8_ETC2_EAC;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2RGB_Alpha1:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ETC2SRGB_Alpha1:
        if (m_compressedTextureETC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureETC->COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC4x4:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_4x4_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC6x6:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_6x6_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC8x8:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_8x8_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC10x10:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_10x10_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC12x12:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_RGBA_ASTC_12x12_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC4x4SRGB:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC6x6SRGB:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC8x8SRGB:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC10x10SRGB:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case TextureFormat::ASTC12x12SRGB:
        if (m_compressedTextureASTC != nullptr)
        {
            m_glParam.internalFormat = m_compressedTextureASTC->COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    default:
        assert("Unknown Texture Format.");
        break;
    }
    return m_glParam;
}

// todo srgb ?
const GLParam &GLTextureContext::glRenderTextureParam(RenderTargetFormat format, bool useSRGB)
{
    m_glParam.internalFormat = 0;
    m_glParam.format = 0;
    m_glParam.type = 0;

    switch (format)
    {
    case RenderTargetFormat::R8G8B8:
        if (m_sRGB != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_sRGB->SRGB_EXT : GL_RGB;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case RenderTargetFormat::R8G8B8A8:
        if (m_sRGB != nullptr)
        {
            m_glParam.internalFormat = useSRGB ? m_sRGB->SRGB_EXT : GL_RGBA;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = GL_UNSIGNED_BYTE;
        }
        break;
    case RenderTargetFormat::R16G16B16:
        if (m_oesTextureHalfFloat != nullptr)
        {
            m_glParam.internalFormat = GL_RGB;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = m_oesTextureHalfFloat->HALF_FLOAT_OES;
        }
        break;
    case RenderTargetFormat::R16G16B16A16:
        if (m_oesTextureHalfFloat != nullptr)
        {
            m_glParam.internalFormat = GL_RGBA;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = m_oesTextureHalfFloat->HALF_FLOAT_OES;
        }
        break;
    case RenderTargetFormat::R32G32B32:
        m_glParam.internalFormat = GL_RGB;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_FLOAT;
        break;
    case RenderTargetFormat::R32G32B32A32:
        m_glParam.internalFormat = GL_RGBA;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_FLOAT;
        break;
    case RenderTargetFormat::DEPTH_16:
        m_glParam.internalFormat = GL_DEPTH_COMPONENT;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_UNSIGNED_SHORT;
        break;
    case RenderTargetFormat::DEPTHSTENCIL_24_8:
        if (m_webgl_depth_texture != nullptr)
        {
            m_glParam.internalFormat = GL_DEPTH_STENCIL;
            m_glParam.format = m_glParam.internalFormat;
            m_glParam.type = m_webgl_depth_texture->UNSIGNED_INT_24_8_WEBGL;
        }
        break;
    case RenderTargetFormat::DEPTH_32:
        m_glParam.internalFormat = GL_DEPTH_COMPONENT;
        m_glParam.format = m_glParam.internalFormat;
        m_glParam.type = GL_UNSIGNED_INT;
        break;
    case RenderTargetFormat::STENCIL_8:
    default:
        assert("render texture format wrong.");
        break;
    }

    return m_glParam;
}

GLRenderBufferParam *GLTextureContext::glRenderBufferParam(RenderTargetFormat format, bool useSRGB)
{
    static GLRenderBufferParam value;
    // todo
    switch (format)
    {
    case RenderTargetFormat::DEPTH_16:
        value.internalFormat = GL_DEPTH_COMPONENT16;
        value.attachment = GL_DEPTH_ATTACHMENT;
        return &value;
    case RenderTargetFormat::DEPTHSTENCIL_24_8:
        value.internalFormat = GL_DEPTH24_STENCIL8; // GL_DEPTH_STENCIL
        value.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        return &value;
    case RenderTargetFormat::DEPTH_32:
        value.internalFormat = GL_DEPTH_STENCIL;
        value.attachment = GL_DEPTH_ATTACHMENT;
        return &value;
    case RenderTargetFormat::STENCIL_8:
        value.internalFormat = GL_STENCIL_INDEX8;
        value.attachment = GL_STENCIL_ATTACHMENT;
        return &value;
    default:
        return nullptr;
    }
}

GLenum GLTextureContext::glRenderTargetAttachment(RenderTargetFormat format)
{
    switch (format)
    {
    case RenderTargetFormat::DEPTH_16:
        return GL_DEPTH_ATTACHMENT;
    case RenderTargetFormat::DEPTHSTENCIL_24_8:
        return GL_DEPTH_STENCIL_ATTACHMENT;
    case RenderTargetFormat::DEPTH_32:
        return GL_DEPTH_ATTACHMENT;
    case RenderTargetFormat::STENCIL_8:
        return GL_STENCIL_ATTACHMENT;
    case RenderTargetFormat::R8G8B8:
    case RenderTargetFormat::R8G8B8A8:
    case RenderTargetFormat::R16G16B16:
    case RenderTargetFormat::R16G16B16A16:
    case RenderTargetFormat::R32G32B32:
    case RenderTargetFormat::R32G32B32A32:
        return GL_COLOR_ATTACHMENT0;
    default:
        assert("render format.");
        return GL_NONE;
    }
}

GLenum GLTextureContext::getTarget(TextureDimension dimension)
{
    switch (dimension)
    {
    case TextureDimension::Tex2D:
        return GL_TEXTURE_2D;
    case TextureDimension::Cube:
        return GL_TEXTURE_CUBE_MAP;
    default:
        assert("texture dimension wrong in WebGL1.");
        return GL_NONE;
    }
}

int GLTextureContext::getGLtexMemory(GLESInternalTex *tex, int depth)
{
    int channels = 0;
    int singlebyte = 0;
    int bytelength = 0;
    switch (tex->m_internalFormat)
    {
    case 0x8C40: // m_sRGB->SRGB_EXT:
    case GL_RGB:
        channels = 3;
        break;
    case 0x8C42: // m_sRGB->SRGB_ALPHA_EXT:
    case GL_RGBA:
        channels = 4;
        break;
    default:
        channels = 0;
        break;
    }
    switch (tex->m_type)
    {
    case GL_UNSIGNED_BYTE:
        singlebyte = 1;
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
        singlebyte = 2 / 3;
        break;
    case GL_FLOAT:
        singlebyte = 4;
        break;
    case 36193: // m_oesTextureHalfFloat->HALF_FLOAT_OES:
        singlebyte = 2;
        break;
    default:
        singlebyte = 0;
        break;
    }
    bytelength = channels * singlebyte * tex->getWidth() * tex->getHeight();
    if (tex->mipmap())
    {
        bytelength *= 1.333;
    }
    if (tex->m_target == GL_TEXTURE_CUBE_MAP)
        bytelength *= 6;
    else if (tex->m_target == GL_TEXTURE_2D)
        bytelength *= 1;

    return bytelength;
}
static int getpixelbyte(RenderTargetFormat rtFormat)
{
    int pixelByte = 0;
    switch (rtFormat)
    {
    case RenderTargetFormat::R8G8B8:
        pixelByte = 3;
        break;
    case RenderTargetFormat::R8G8B8A8:
        pixelByte = 4;
        break;
    case RenderTargetFormat::R16G16B16A16:
        pixelByte = 8;
        break;
    case RenderTargetFormat::R32G32B32:
        pixelByte = 12;
        break;
    case RenderTargetFormat::R32G32B32A32:
        pixelByte = 16;
        break;
    case RenderTargetFormat::R16G16B16:
        pixelByte = 6;
        break;
    case RenderTargetFormat::DEPTH_16:
        pixelByte = 2;
        break;
    case RenderTargetFormat::STENCIL_8:
        pixelByte = 1;
        break;
    case RenderTargetFormat::DEPTHSTENCIL_24_8:
        pixelByte = 4;
        break;
    case RenderTargetFormat::DEPTH_32:
        pixelByte = 4;
        break;
    }
    return pixelByte;
}
int GLTextureContext::getGLRTTexMemory(int width, int height, RenderTargetFormat colorFormat,
                                       RenderTargetFormat depthStencilFormat, bool generateMipmap, int multiSamples,
                                       bool cube)
{
    int colorPixelbyte = getpixelbyte(colorFormat);
    int depthPixelbyte = getpixelbyte(depthStencilFormat);
    if (multiSamples > 1) // 多重采样
        colorPixelbyte *= 2;
    if (cube) // box
        colorPixelbyte *= 6;
    if (generateMipmap) // mipmap
        colorPixelbyte *= 1.333;
    int colorMemory = colorPixelbyte * width * height;
    int depthMemory = depthPixelbyte * width * height;
    return colorMemory + depthMemory;
}

// protected getRenderTargetDepthFormat(format: RenderTargetDepthFormat): { internalFormat: number, attachment: number }
// {
//     let gl = this.gl;
//     switch (format) {
//         case RenderTargetDepthFormat.DEPTH_16:
//             return { internalFormat: gl.DEPTH_COMPONENT16, attachment: gl.DEPTH_ATTACHMENT };
//         case RenderTargetDepthFormat.DEPTHSTENCIL_24_8:
//             return { internalFormat: gl.DEPTH_STENCIL, attachment: gl.DEPTH_STENCIL_ATTACHMENT };
//         case RenderTargetDepthFormat.DEPTH_32:
//             return { internalFormat: gl.DEPTH_STENCIL, attachment: gl.DEPTH_STENCIL_ATTACHMENT };
//         case RenderTargetDepthFormat.STENCIL_8:
//             return { internalFormat: gl.STENCIL_INDEX8, attachment: gl.STENCIL_ATTACHMENT };
//         case RenderTargetDepthFormat.DEPTHSTENCIL_NONE:
//             return null;
//         default:
//             throw "RenderTargetDepthFormat wrong."
//     }
// }

bool GLTextureContext::supportSRGB(int format /*TextureFormat | RenderTargetFormat*/, bool mipmap)
{
    switch ((TextureFormat)format)
    {
    case TextureFormat::R8G8B8:
    case TextureFormat::R8G8B8A8:
        return m_engine->getCapable(RenderCapable::Texture_SRGB) && !mipmap;
    case TextureFormat::DXT1:
    case TextureFormat::DXT3:
    case TextureFormat::DXT5:
        // todo  验证 srgb format 和 mipmap webgl1 兼容问题
        return m_engine->getCapable(RenderCapable::COMPRESS_TEXTURE_S3TC_SRGB) && !mipmap;
    default:
        return false;
    }
}

bool GLTextureContext::supportGenerateMipmap(int format /*TextureFormat | RenderTargetFormat*/)
{
    switch ((RenderTargetFormat)format)
    {
    case RenderTargetFormat::DEPTH_16:
    case RenderTargetFormat::DEPTHSTENCIL_24_8:
    case RenderTargetFormat::DEPTH_32:
    case RenderTargetFormat::STENCIL_8:
        return false;
    default:
        return true;
    }
}
bool GLTextureContext::isSRGBFormat(int format /*TextureFormat | RenderTargetFormat*/)
{
    switch ((TextureFormat)format)
    {
    case TextureFormat::ETC2SRGB:
    case TextureFormat::ETC2SRGB_Alpha8:
    case TextureFormat::ASTC4x4SRGB:
    case TextureFormat::ASTC6x6SRGB:
    case TextureFormat::ASTC8x8SRGB:
    case TextureFormat::ASTC10x10SRGB:
    case TextureFormat::ASTC12x12SRGB:
        return true;
    default:
        return false;
    }
}

GLESInternalTex *GLTextureContext::createTextureInternal(TextureDimension dimension, int width, int height,
                                                         TextureFormat format, bool gengerateMipmap, bool sRGB,
                                                         bool premultipliedAlpha)
{

    // todo  一些format 不支持自动生成mipmap

    // todo  这个判断, 若纹理本身格式不支持？
    bool useSRGBExt = isSRGBFormat((int)format) || (sRGB && supportSRGB((int)format, gengerateMipmap));
    if (premultipliedAlpha)
    { // 预乘法和SRGB同时开启，会有颜色白边问题
        useSRGBExt = false;
    }
    float gammaCorrection = 1.0f;
    if (!useSRGBExt && sRGB)
    {
        gammaCorrection = 2.2f;
    }

    // let dimension = TextureDimension.Tex2D;
    GLenum target = getTarget(dimension);
    GLESInternalTex *internalTex =
        new GLESInternalTex(target, width, height, 1, dimension, gengerateMipmap, useSRGBExt, gammaCorrection);

    const GLParam &glParam = glTextureParam(format, useSRGBExt);

    internalTex->m_internalFormat = glParam.internalFormat;
    internalTex->m_format = glParam.format;
    internalTex->m_type = glParam.type;

    // Resource:
    // internalTex.gpuMemory = this.getGLtexMemory(format, width, height, dimension, gengerateMipmap);

    return internalTex;
}

void GLTextureContext::setTextureImageData(GLESInternalTex *texture,
                                           JCImage *source /*HTMLImageElement | HTMLCanvasElement | ImageBitmap*/,
                                           bool premultiplyAlpha, bool invertY)
{
    source->updateTexImage();
    if (texture->m_width != source->getWidth() || texture->m_height != source->getHeight())
    {
        // todo ?
        // console.warn("setTextureImageData: size not match");
    }

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    if (premultiplyAlpha)
    {
        source->premultiplyAlpha();
    }
    if (invertY)
    {
        JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, width, height, source->m_kBitmapData.m_pImageData);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    char *pBufferData = (char *)source->m_kBitmapData.m_pImageData;
    if (pBufferData && width > 0 && height > 0)
    {
        unsigned char *outData = nullptr;
        size_t outDataLen = 0;
        convertRGBA8888ToFormat((unsigned char *)pBufferData,
                                source->m_kBitmapData.m_nWidth * source->m_kBitmapData.m_nHeight * 4, format, &outData,
                                &outDataLen);
        // gl.texImage2D(target, 0, internalFormat, format, type, source);
        TexImage2DSafe(target, 0, internalFormat, width, height, 0, format, type, outData);
        texture->setGpuMemory(getGLtexMemory(texture));
        if (outData != (unsigned char *)pBufferData)
        {
            delete[] outData;
        }
    }

    // gl.texImage2D(target, 0, internalFormat, width, height, 0, format, type, null);
    // gl.texSubImage2D(target, 0, 0, 0, format, type, source);

    if (texture->mipmap())
    {
        glGenerateMipmap(texture->m_target);
    }
    m_engine->_bindTexture(texture->m_target, 0);
}
// setTextureSubImageData TODO
void GLTextureContext::setTexturePixelsData(GLESInternalTex *texture, char *source, int sourceBytes,
                                            bool premultiplyAlpha, bool invertY)
{
    // todo check pixels size

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    int fourSize = width % 4 == 0 && height % 4 == 0;

    if (premultiplyAlpha)
    {
        JCImage::premultiplyPixels((const GLubyte *)source, (GLubyte *)source, sourceBytes, type, format);
    }
    if (invertY)
    {
        JCImage::flipY(type, format, width, height, source);
    }
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    // gl.texImage2D(target, 0, internalFormat, format, type, source);

    TexImage2DSafe(target, 0, internalFormat, width, height, 0, format, type, source);
    // gl.texSubImage2D(target, 0, 0, 0, format, type, source);
    texture->setGpuMemory(getGLtexMemory(texture));

    if (texture->mipmap())
    {
        glGenerateMipmap(texture->m_target);
    }
    m_engine->_bindTexture(texture->m_target, 0);
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

void GLTextureContext::initVideoTextureData(GLESInternalTex *texture)
{
    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    m_engine->_bindTexture(target, texture);
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);
    texture->setGpuMemory(getGLtexMemory(texture));
    if (texture->mipmap())
    {
        glGenerateMipmap(texture->m_target);
    }

    m_engine->_bindTexture(target, nullptr);
}

void GLTextureContext::setTextureSubPixelsData(GLESInternalTex *texture, char *source, int mipmapLevel,
                                               bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                               bool premultiplyAlpha, bool invertY)
{

    generateMipmap = generateMipmap && mipmapLevel == 0;

    // todo check pixels size

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    // let width = texture.width;
    // let height = texture.height;

    int fourSize = width % 4 == 0 && height % 4 == 0;

    if (premultiplyAlpha)
    {
        int sourceBytes = width * height * getBytesPerPixel(type, format);
        JCImage::premultiplyPixels((const GLubyte *)source, (GLubyte *)source, sourceBytes, type, format);
    }
    if (invertY)
    {
        JCImage::flipY(type, format, width, height, source);
    }
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    // gl.texImage2D(target, 0, internalFormat, format, type, null);

    // gl.texImage2D(target, 0, internalFormat, width, height, 0, format, type, source);
    glTexSubImage2D(target, mipmapLevel, xOffset, yOffset, width, height, format, type, source);

    if (texture->mipmap() && generateMipmap)
    {
        glGenerateMipmap(texture->m_target);
    }
    m_engine->_bindTexture(texture->m_target, 0);
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}
void GLTextureContext::setTextureDDSData(GLESInternalTex *texture, const DDSTextureInfo &ddsInfo)
{

    bool premultiplyAlpha = false;
    bool invertY = false;

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    const char *source = ddsInfo.source;
    int dataOffset = ddsInfo.dataOffset;
    int bpp = ddsInfo.bpp;
    int blocksBytes = ddsInfo.blockBytes;
    int mipmapCount = ddsInfo.mipmapCount;
    bool compressed = ddsInfo.compressed;

    texture->setMaxMipmapLevel(mipmapCount - 1);
    bool fourSize = width % 4 == 0 && height % 4 == 0;

    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    FormatPixelsParams formatParams;
    getFormatPixelsParams(ddsInfo.format, formatParams);
    int channelsByte = compressed ? 0 : formatParams.bytesPerPixel / formatParams.channels;
    //let dataTypeConstur = formatParams.dataTypedCons;


    int mipmapWidth = width;
    int mipmapHeight = height;
    int memory = 0;

    for (int index = 0; index < mipmapCount; index++)
    {
        if (compressed) {
            int32_t dataLength = std::max(4, mipmapWidth) / 4 * std::max(4, mipmapHeight) / 4 * blocksBytes;
            glCompressedTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, dataLength, source + dataOffset);

            memory += dataLength;
            dataOffset += bpp ? (mipmapWidth * mipmapHeight * (bpp / 8)) : dataLength;
        }
        else {
            int dataLength = mipmapWidth * mipmapHeight * formatParams.channels;
            memory += dataLength;
            glTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, format, type, source+ dataOffset);
            
            dataOffset += dataLength * channelsByte;
        }
       

        mipmapWidth *= 0.5;
        mipmapHeight *= 0.5;
        mipmapWidth = std::max(1, (int)mipmapWidth);
        mipmapHeight = std::max(1, (int)mipmapHeight);
    }
    texture->setGpuMemory(memory);
    m_engine->_bindTexture(texture->m_target, nullptr);
    
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}
void GLTextureContext::setTextureKTXData(GLESInternalTex *texture, const KTXTextureInfo &ktxInfo) // TODO
{
    int width = texture->m_width;
    int height = texture->m_height;

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;

    const char *source = ktxInfo.source;
    bool compressd = ktxInfo.compress;
    bool fourSize = width % 4 == 0 && height % 4 == 0;

    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    int mipmapWidth = width;
    int mipmapHeight = height;
    int dataOffset = ktxInfo.headerOffset + ktxInfo.bytesOfKeyValueData;
    int memory = 0;

    source += dataOffset;
    for (int index = 0; index < ktxInfo.mipmapCount; index++)
    {
        int32_t imageSize = static_cast<int32_t *>((void *)(source))[0];
        source += 4;
        if (compressd)
        {
            glCompressedTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, imageSize, source);
        }
        else
        {
            glTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, format, type, source);
        }

        memory += imageSize;
        source += imageSize;
        source += 3 - (imageSize + 3) % 4;
        mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
        mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
    }

    m_engine->_bindTexture(texture->m_target, nullptr);
    texture->setGpuMemory(memory);
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

/*setTextureHDRData(texture: GLESInternalTex, hdrInfo: HDRTextureInfo): void {
TODO
}*/
void GLTextureContext::setCubeImageData(GLESInternalTex *texture, const std::vector<JSImage *> &sources,
                                        bool premultiplyAlpha, bool invertY)
{
    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };

    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    m_engine->_bindTexture(texture->m_target, texture);

    for (int index = 0; index < 6; index++)
    {
        GLenum target = cubeFace[index];
        auto pImage = sources[index]->m_pImage;

        pImage->updateTexImage();
        if (premultiplyAlpha)
        {
            pImage->premultiplyAlpha();
        }
        if (invertY)
        {
            JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, pImage->m_kBitmapData.m_nWidth, pImage->m_kBitmapData.m_nHeight,
                           pImage->m_kBitmapData.m_pImageData);
        }
        char *pBufferData = (char *)pImage->m_kBitmapData.m_pImageData;
        if (pBufferData && width > 0 && height > 0)
        {
            unsigned char *outData = nullptr;
            size_t outDataLen = 0;
            convertRGBA8888ToFormat((unsigned char *)pBufferData,
                                    pImage->m_kBitmapData.m_nWidth * pImage->m_kBitmapData.m_nHeight * 4, format,
                                    &outData, &outDataLen);
            TexImage2DSafe(target, 0, internalFormat, width, height, 0, format, type, outData);
            if (outData != (unsigned char *)pBufferData)
            {
                delete[] outData;
            }
        }
    }

    if (texture->mipmap())
    {
        glGenerateMipmap(texture->m_target);
    }
    m_engine->_bindTexture(texture->m_target, 0);
    texture->setGpuMemory(getGLtexMemory(texture));
}

void GLTextureContext::setCubePixelsData(GLESInternalTex *texture, const std::vector<char *> &source,
                                         bool premultiplyAlpha, bool invertY)
{
    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    bool fourSize = width % 4 == 0;
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);
    if (!source.empty())
    {
        for (int index = 0; index < 6; index++)
        {
            GLenum t = cubeFace[index];
            if (premultiplyAlpha)
            {
                int sourceBytes = width * height * getBytesPerPixel(type, format);
                JCImage::premultiplyPixels((const GLubyte *)source[index], (GLubyte *)source[index], sourceBytes, type,
                                           format);
            }
            if (invertY)
            {
                JCImage::flipY(type, format, width, height, source[index]);
            }
            // gl.texImage2D(t, 0, internalFormat, format, type, sources[index]);
            TexImage2DSafe(t, 0, internalFormat, width, height, 0, format, type, source[index]);
        }
        if (texture->mipmap())
        {
            glGenerateMipmap(texture->m_target);
        }
    }
    else
    {
        for (int index = 0; index < 6; index++)
        {
            GLenum t = cubeFace[index];
            // gl.texImage2D(t, 0, internalFormat, format, type, sources[index]);
            TexImage2DSafe(t, 0, internalFormat, width, height, 0, format, type, 0);
        }
        if (texture->mipmap())
        {
            glGenerateMipmap(texture->m_target);
        }
    }
    m_engine->_bindTexture(texture->m_target, 0);
    texture->setGpuMemory(getGLtexMemory(texture));
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

void GLTextureContext::setCubeSubPixelData(GLESInternalTex *texture, const std::vector<char *> &source, int mipmapLevel,
                                           bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                           bool premultiplyAlpha, bool invertY)
{

    generateMipmap = generateMipmap && mipmapLevel == 0;

    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;

    bool fourSize = width % 4 == 0;
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    for (int index = 0; index < 6; index++)
    {
        GLenum target = cubeFace[index];
        if (premultiplyAlpha)
        {
            int sourceBytes = width * height * getBytesPerPixel(type, format);
            JCImage::premultiplyPixels((const GLubyte *)source[index], (GLubyte *)source[index], sourceBytes, type,
                                       format);
        }
        if (invertY)
        {
            JCImage::flipY(type, format, width, height, source[index]);
        }
        // gl.texImage2D(target, 0, internalFormat, format, type, sources[index]);
        glTexSubImage2D(target, mipmapLevel, xOffset, yOffset, width, height, format, type, source[index]);
    }

    if (texture->mipmap() && generateMipmap)
    {
        glGenerateMipmap(texture->m_target);
    }
    m_engine->_bindTexture(texture->m_target, 0);
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

FormatParam getFomatPixelsParams(TextureFormat format)
{
    FormatParam formatParam = {0, 0, 1};

    switch (format)
    {
    case TextureFormat::R8G8B8A8:
        formatParam.channels = 4;
        formatParam.bytesPerPixel = 4;
        formatParam.typesSize = 1;
        break;
    case TextureFormat::R8G8B8:
        formatParam.channels = 3;
        formatParam.bytesPerPixel = 3;
        formatParam.typesSize = 1;
    case TextureFormat::R5G6B5:
        formatParam.channels = 3;
        formatParam.bytesPerPixel = 2;
        formatParam.typesSize = 2;
    case TextureFormat::R16G16B16:
        formatParam.channels = 3;
        formatParam.bytesPerPixel = 6;
        formatParam.typesSize = 2;
    case TextureFormat::R16G16B16A16:
        formatParam.channels = 4;
        formatParam.bytesPerPixel = 8;
        formatParam.typesSize = 2;
    case TextureFormat::R32G32B32:
        formatParam.channels = 3;
        formatParam.bytesPerPixel = 12;
        formatParam.typesSize = 4;
    case TextureFormat::R32G32B32A32:
        formatParam.channels = 4;
        formatParam.bytesPerPixel = 16;
        formatParam.typesSize = 4;
    default:
        break;
    }
    return formatParam;
}

void GLTextureContext::setCubeDDSData(GLESInternalTex *texture, const DDSTextureInfo &ddsInfo)
{

    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    const char *source = ddsInfo.source;
    int dataOffset = ddsInfo.dataOffset;
    int bpp = ddsInfo.bpp;
    int blockBytes = ddsInfo.blockBytes;
    int mipmapCount = texture->m_mipmapCount;

    bool fourSize = width % 4 == 0 && height % 4 == 0;
    fourSize = true;
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };

    FormatParam formatParams = getFomatPixelsParams(ddsInfo.format);
    int channelsByte = formatParams.bytesPerPixel / formatParams.channels;

    int memory = 0;
    source += dataOffset;

    if (!ddsInfo.compressed)
    {
        for (int index = 0; index < 6; index++)
        {
            GLenum target = cubeFace[index];
            int mipmapWidth = width;
            int mipmapHeight = height;
            for (int index = 0; index < mipmapCount; index++)
            {
                glTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, format, type, source);
                int dataLength = mipmapWidth * mipmapHeight * formatParams.bytesPerPixel;
                memory += dataLength;
                source += dataLength;
                mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
                mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
            }
        }
    }
    else
    {
        for (int index = 0; index < 6; index++)
        {
            GLenum target = cubeFace[index];
            int mipmapWidth = width;
            int mipmapHeight = height;
            for (int index = 0; index < mipmapCount; index++)
            {
                int32_t imageSize = std::max(4, mipmapWidth) / 4 * std::max(4, mipmapWidth) / 4 * blockBytes;
                glCompressedTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, imageSize, source);
                memory += imageSize;
                source += bpp ? (mipmapWidth * mipmapHeight * (bpp / 8)) : imageSize;
                mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
                mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
            }
        }
    }

    m_engine->_bindTexture(texture->m_target, 0);
    texture->setGpuMemory(getGLtexMemory(texture));
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}
void GLTextureContext::setCubeKTXData(GLESInternalTex *texture, const KTXTextureInfo &ktxInfo)
{
    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };
    bool compressed = ktxInfo.compress;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int mipmapCount = texture->m_mipmapCount;
    int width = texture->m_width;
    int height = texture->m_height;
    const char *source = ktxInfo.source;

    bool fourSize = width % 4 == 0 && height % 4 == 0;
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    m_engine->_bindTexture(texture->m_target, texture);

    int mipmapWidth = width;
    int mipmapHeight = height;
    int dataOffset = ktxInfo.headerOffset + ktxInfo.bytesOfKeyValueData;
    int memory = 0;
    source += dataOffset;

    for (int index = 0; index < mipmapCount; index++)
    {
        int32_t imageSize = static_cast<int32_t *>((void *)(source))[0];
        source += 4;
        for (int index = 0; index < 6; index++)
        {
            GLenum target = cubeFace[index];
            if (ktxInfo.compress)
            {
                glCompressedTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, imageSize, source);
            }
            else
            {
                glTexImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, format, type, source);
            }

            memory += imageSize;
            source += imageSize;
            source += 3 - (imageSize + 3) % 4;
        }

        mipmapWidth = std::max(1, (int)(mipmapWidth * 0.5));
        mipmapHeight = std::max(1, (int)(mipmapHeight * 0.5));
    }
    for (int index = ktxInfo.mipmapCount; index < texture->m_mipmapCount; index++) 
    {
        for (int face = 0; face < 6; face++) {
            int target = cubeFace[face];
            if (compressed) {
                // todo 
            }
            else {
                glTexSubImage2D(target, index, internalFormat, mipmapWidth, mipmapHeight, 0, format, type, 0);
            }
        }

        mipmapWidth = std::max(1.0, mipmapWidth * 0.5);
        mipmapHeight = std::max(1.0, mipmapHeight * 0.5);
    }

    m_engine->_bindTexture(texture->m_target, 0);
    texture->setGpuMemory(getGLtexMemory(texture));
    if (!fourSize)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

TextureCompareMode GLTextureContext::setTextureCompareMode(GLESInternalTex *texture, TextureCompareMode compareMode)
{
    return TextureCompareMode::None;
}

void GLTextureContext::bindRenderTarget(GLESInternalRT *renderTarget, int faceIndex)
{
    if (renderTarget != currentActiveRT && currentActiveRT != nullptr) {
        unbindRenderTarget(currentActiveRT);
    }
    auto framebuffer = renderTarget->m_framebuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    if (renderTarget->m_isCube)
    {
        GLESInternalTex* texture = renderTarget->m_textures[0];
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
                               texture->m_resource, 0);
    }
    currentActiveRT = renderTarget;
}

void GLTextureContext::bindoutScreenTarget()
{
    if(currentActiveRT!=nullptr)
    unbindRenderTarget(currentActiveRT);
    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
  
}

void GLTextureContext::unbindRenderTarget(GLESInternalRT *renderTarget)
{
    if (renderTarget->m_generateMipmap)
    {
        for (GLESInternalRT::TexturesVec::iterator it = renderTarget->m_textures.begin();
             it != renderTarget->m_textures.end(); it++)
        {
            GLESInternalTex *tex = *it;
            if (tex)
            {
                m_engine->_bindTexture(tex->m_target, tex);
                glGenerateMipmap(tex->m_target);
                m_engine->_bindTexture(tex->m_target, 0);
            }
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
    currentActiveRT = nullptr;
}

GLESInternalTex *GLTextureContext::createRenderTextureInternal(TextureDimension dimension, int width, int height,
                                                               RenderTargetFormat format, bool generateMipmap,
                                                               bool sRGB)
{
    bool useSRGBExt = false;

    generateMipmap = generateMipmap && supportGenerateMipmap((int)format);

    float gammaCorrection = 1.0f;
    // if (!useSRGBExt && sRGB) {
    //     gammaCorrection = 2.2f;
    // }

    // let dimension = TextureDimension.Tex2D;
    int target = getTarget(dimension);
    GLESInternalTex *internalTex =
        new GLESInternalTex(target, width, height, 1, dimension, generateMipmap, useSRGBExt, gammaCorrection);

    const GLParam &glParam = glRenderTextureParam(format, useSRGBExt);

    internalTex->m_internalFormat = glParam.internalFormat;
    internalTex->m_format = glParam.format;
    internalTex->m_type = glParam.type;

    int internalFormat = internalTex->m_internalFormat;
    int glFormat = internalTex->m_format;
    int type = internalTex->m_type;

    m_engine->_bindTexture(internalTex->m_target, internalTex);

    TexImage2DSafe(target, 0, internalFormat, width, height, 0, glFormat, type, 0);
    // internalTex.gpuMemory = this.getGLtexMemory(internalTex);
    m_engine->_bindTexture(internalTex->m_target, 0);

    if (format == RenderTargetFormat::DEPTH_16 || format == RenderTargetFormat::DEPTH_32 ||
        format == RenderTargetFormat::DEPTHSTENCIL_24_8)
    {
        internalTex->setFilterMode(FilterMode::Point);
    }

    return internalTex;
}

GLESInternalTex *GLTextureContext::createRenderTextureCubeInternal(TextureDimension dimension, int size,
                                                                   RenderTargetFormat format, bool generateMipmap,
                                                                   bool sRGB)
{
    bool useSRGBExt = false;

    generateMipmap = generateMipmap && supportGenerateMipmap((int)format);

    float gammaCorrection = 1.0f;
    // if (!useSRGBExt && sRGB)
    //{
    //     gammaCorrection = 2.2f;
    // }

    GLenum target = getTarget(dimension);
    GLESInternalTex *internalTex =
        new GLESInternalTex(target, size, size, 1, dimension, generateMipmap, useSRGBExt, gammaCorrection);

    const GLParam &glParam = glRenderTextureParam(format, useSRGBExt);

    internalTex->m_internalFormat = glParam.internalFormat;
    internalTex->m_format = glParam.format;
    internalTex->m_type = glParam.type;

    int internalFormat = internalTex->m_internalFormat;
    int glFormat = internalTex->m_format;
    int type = internalTex->m_type;

    const GLenum cubeFace[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // back
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // front
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, // right
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // left
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // up
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // down
    };

    m_engine->_bindTexture(internalTex->m_target, internalTex);

    for (int index = 0; index < 6; index++)
    {
        GLenum target = cubeFace[index];
        TexImage2DSafe(target, 0, internalFormat, size, size, 0, glFormat, type, 0);
    }
    m_engine->_bindTexture(internalTex->m_target, 0);
    // internalTex.gpuMemory = this.getGLtexMemory(internalTex);
    if (format == RenderTargetFormat::DEPTH_16 || format == RenderTargetFormat::DEPTH_32 ||
        format == RenderTargetFormat::DEPTHSTENCIL_24_8)
    {
        internalTex->m_filterMode = FilterMode::Point;
    }

    return internalTex;
}

GLESInternalRT *GLTextureContext::createRenderTargetInternal(int width, int height, RenderTargetFormat colorFormat,
                                                             RenderTargetFormat depthStencilFormat, bool generateMipmap,
                                                             bool sRGB, int multiSamples, bool storage)
{
    multiSamples = 1;

    GLESInternalTex *texture =
        createRenderTextureInternal(TextureDimension::Tex2D, width, height, colorFormat, generateMipmap, sRGB);

    GLESInternalRT *renderTarget =
        new GLESInternalRT(colorFormat, depthStencilFormat, false, texture->mipmap(), multiSamples);
    renderTarget->setGpuMemory(
        getGLRTTexMemory(width, height, colorFormat, depthStencilFormat, generateMipmap, multiSamples, true));
    renderTarget->m_colorFormat = colorFormat;
    renderTarget->m_depthStencilFormat = depthStencilFormat;
    renderTarget->m_textures.push_back(texture);

    GLuint framebuffer = renderTarget->m_framebuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // color
    GLenum colorAttachment = glRenderTargetAttachment(colorFormat);
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, texture->m_resource, 0);
    // depth
    GLRenderBufferParam *depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
    if (depthBufferParam)
    {
        GLuint depthbuffer =
            createRenderbuffer(width, height, depthBufferParam->internalFormat, renderTarget->m_samples);
        renderTarget->m_depthbuffer = depthbuffer;
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);

    return renderTarget;
}

GLESInternalRT *GLTextureContext::createRenderTargetCubeInternal(int size, RenderTargetFormat colorFormat,
                                                                 RenderTargetFormat depthStencilFormat,
                                                                 bool generateMipmap, bool sRGB, int multiSamples)
{
    multiSamples = 1;

    // let texture = this.createRenderTextureInternal(dimension, size, size, colorFormat, gengerateMipmap, sRGB);
    GLESInternalTex *texture =
        createRenderTextureCubeInternal(TextureDimension::Cube, size, colorFormat, generateMipmap, sRGB);

    GLESInternalRT *renderTarget =
        new GLESInternalRT(colorFormat, depthStencilFormat, true, texture->mipmap(), multiSamples);
    renderTarget->setGpuMemory(
        getGLRTTexMemory(size, size, colorFormat, depthStencilFormat, generateMipmap, multiSamples, true));
    renderTarget->m_textures.push_back(texture);

    GLuint framebuffer = renderTarget->m_framebuffer;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLRenderBufferParam *depthBufferParam = glRenderBufferParam(depthStencilFormat, false);
    if (depthBufferParam)
    {
        GLuint depthbuffer = createRenderbuffer(size, size, depthBufferParam->internalFormat, renderTarget->m_samples);
        renderTarget->m_depthbuffer = depthbuffer;
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBufferParam->attachment, GL_RENDERBUFFER, depthbuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);

    return renderTarget;
}

GLuint GLTextureContext::createRenderbuffer(int width, int height, int internalFormat, int samples)
{

    // todo  多个 gl

    GLuint renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return renderbuffer;
}

// todo  color 0, 1, 2, 3 ?
void GLTextureContext::setupRendertargetTextureAttachment(GLESInternalRT *renderTarget, GLESInternalTex *texture)
{
    renderTarget->m_depthTexture = texture;

    GLuint depthbuffer = renderTarget->m_depthbuffer;
    if (depthbuffer)
    {
        glDeleteRenderbuffers(1, &depthbuffer);
    }
    renderTarget->m_depthbuffer = 0;
    GLenum attachment = glRenderTargetAttachment(renderTarget->m_depthStencilFormat);

    GLuint framebuffer = renderTarget->m_framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->m_resource, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);
}

void GLTextureContext::readRenderTargetPixelData(GLESInternalRT *renderTarget, int xOffset, int yOffset, int width,
                                                 int height, std::vector<uint8_t> &out)
{
    out.clear();
    bindRenderTarget(renderTarget, 0);

    bool frameState = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

    if (!frameState)
    {
        unbindRenderTarget(renderTarget);
        return;
    }
    switch (renderTarget->m_colorFormat)
    {
    case RenderTargetFormat::R8G8B8:
        out.resize(width * height * 3);
        glReadPixels(xOffset, yOffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, &out[0]);
        break;
    case RenderTargetFormat::R8G8B8A8:
        out.resize(width * height * 4);
        glReadPixels(xOffset, yOffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &out[0]);
        break;
    case RenderTargetFormat::R16G16B16:
        out.resize(width * height * 3 * 2);
        glReadPixels(xOffset, yOffset, width, height, GL_RGB, GL_HALF_FLOAT, &out[0]);
        break;
    case RenderTargetFormat::R16G16B16A16:
        out.resize(width * height * 4 * 2);
        glReadPixels(xOffset, yOffset, width, height, GL_RGBA, GL_HALF_FLOAT, &out[0]);
        break;
    case RenderTargetFormat::R32G32B32:
        out.resize(width * height * 3 * 4);
        glReadPixels(xOffset, yOffset, width, height, GL_RGB, GL_FLOAT, &out[0]);
        break;
    case RenderTargetFormat::R32G32B32A32:
        out.resize(width * height * 4 * 4);
        glReadPixels(xOffset, yOffset, width, height, GL_RGBA, GL_FLOAT, &out[0]);
        break;
    }
    unbindRenderTarget(renderTarget);
}

void GLTextureContext::updateVideoTexture(GLESInternalTex *texture, JCImage *source, bool premultiplyAlpha,
                                          bool invertY)
{

    if (source == nullptr)
    {
        return;
    }
    source->updateTexImage();

    int target = texture->m_target;
    int internalFormat = texture->m_internalFormat;
    int format = texture->m_format;
    int type = texture->m_type;
    int width = texture->m_width;
    int height = texture->m_height;

    if (premultiplyAlpha)
    {
        // glPixelStorei(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, true);
        source->premultiplyAlpha();
    }
    if (invertY)
    {
        JCImage::flipY(GL_UNSIGNED_BYTE, GL_RGBA, width, height, source->m_kBitmapData.m_pImageData);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_engine->_bindTexture(texture->m_target, texture);

    char *pBufferData = (char *)source->m_kBitmapData.m_pImageData;
    if (pBufferData && source->m_kBitmapData.m_nWidth > 0 && source->m_kBitmapData.m_nHeight > 0)
    {
        unsigned char *outData = nullptr;
        size_t outDataLen = 0;
        convertRGBA8888ToFormat((unsigned char *)pBufferData,
                                source->m_kBitmapData.m_nWidth * source->m_kBitmapData.m_nHeight * 4, format, &outData,
                                &outDataLen);

        // todo 用 sub 会慢
        // glTexSubImage2D(target, 0, 0, 0, source->m_kBitmapData.m_nWidth, source->m_kBitmapData.m_nHeight, format, type, outData);
        glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, outData);
        texture->setGpuMemory(getGLtexMemory(texture));
        if (outData != (unsigned char *)pBufferData)
        {
            delete[] outData;
        }
    }

    m_engine->_bindTexture(texture->m_target, nullptr);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void GLTextureContext::setTexturePixelsDataJS(GLESInternalTex *texture, jsbind::ArrayBuffer arrayBuffer, bool premultiplyAlpha,
                                              bool invertY)
{
    if (texture)
    {
        if (arrayBuffer.isValid())
        {
            this->setTexturePixelsData(texture, reinterpret_cast<char*>(arrayBuffer.getData()), arrayBuffer.getByteLength(), premultiplyAlpha, invertY);
        }
        else
        {
            this->setTexturePixelsData(texture, nullptr, 0, premultiplyAlpha, invertY);
        }
    }
}
void GLTextureContext::setTextureSubPixelsDataJS(GLESInternalTex *texture, jsbind::ArrayBuffer source, int mipmapLevel,
                                                 bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                                 bool premultiplyAlpha, bool invertY)
{
    DEBUG_CHECK(source.isValid());
    this->setTextureSubPixelsData(texture, reinterpret_cast<char*>(source.getData()), mipmapLevel, generateMipmap, xOffset, yOffset, width,
                                      height, premultiplyAlpha, invertY);
}
void GLTextureContext::setCubePixelsDataJS(GLESInternalTex *texture, std::vector<jsbind::ArrayBuffer> source, bool premultiplyAlpha,
                                           bool invertY)
{
    std::vector<char *> vecDatas;
    for (int i = 0, size = source.size(); i < size; i++)
    {
        char *pArrayBufferPtr = reinterpret_cast<char*>(source[i].getData());
        vecDatas.push_back(pArrayBufferPtr);
    }
    this->setCubePixelsData(texture, vecDatas, premultiplyAlpha, invertY);
}
void GLTextureContext::setCubeSubPixelDataJS(GLESInternalTex *texture, std::vector<jsbind::ArrayBuffer> source, int mipmapLevel,
                                             bool generateMipmap, int xOffset, int yOffset, int width, int height,
                                             bool premultiplyAlpha, bool invertY)
{
    std::vector<char *> vecDatas;
    for (int i = 0, size = source.size(); i < size; i++)
    {
        char *pArrayBufferPtr = reinterpret_cast<char*>(source[i].getData());
        vecDatas.push_back(pArrayBufferPtr);

    }
    this->setCubeSubPixelData(texture, vecDatas, mipmapLevel, generateMipmap, xOffset, yOffset, width, height,
                              premultiplyAlpha, invertY);
}
GLESInternalTex* GLTextureContext::createRenderTargetDepthTexture(GLESInternalRT* renderTarget, TextureDimension dimension, int width, int height)
{
   // let gl = renderTarget._gl;

    if (renderTarget->m_depthStencilFormat == RenderTargetFormat::None) {
        return nullptr;
    }

    // delete depth buffer
    if (renderTarget->m_depthbuffer != 0) {
        glDeleteRenderbuffers(1, &(renderTarget->m_depthbuffer));
        renderTarget->m_depthbuffer = 0;
    }
    // create depth texture
    RenderTargetFormat format = renderTarget->m_depthStencilFormat;
    bool mipmap = renderTarget->m_generateMipmap;
    bool sRGB = renderTarget->_isSRGB;

    // delete old tex
    if (renderTarget->m_depthTexture!=nullptr) {
        renderTarget->m_depthTexture->dispose();
    }
    GLESInternalTex* texture = createRenderTextureInternal(dimension, width, height, format, mipmap, sRGB);
    renderTarget->m_depthTexture = texture;

    // set attachment
    GLenum attachment = glRenderTargetAttachment(renderTarget->m_depthStencilFormat);
    GLuint framebuffer = renderTarget->m_framebuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,GL_TEXTURE_2D, texture->m_resource, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, g_nMainFrameBuffer);

    return texture;
}
} // namespace laya

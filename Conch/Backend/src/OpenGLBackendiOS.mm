#include "OpenGLBackendiOS.h"
#import <GLKit/GLKit.h>
#define GLES_SILENCE_DEPRECATION
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <OpenGLES/ES2/glext.h>
#include <utils/Log.h>

namespace laya {
extern int g_nRealMainFrameBuffer;
struct OpenGLBackendiOSImpl {
    EAGLContext* m_context = nullptr;
    CAEAGLLayer* m_layer = nullptr;
    GLuint m_defaultFramebuffer = 0;
    GLuint m_defaultColorRenderbuffer = 0;
    GLuint m_defaultDepthStencilRenderbuffer = 0;
    GLuint m_msaaFramebuffer = 0;
    GLuint m_msaaRenderbuffer = 0;
    NSString *colorFormat;
    BackendOptions m_options;
};
OpenGLBackendiOS::OpenGLBackendiOS(): m_impl(new OpenGLBackendiOSImpl()) {
    
}
OpenGLBackendiOS::~OpenGLBackendiOS()
{
    destroyScreenSurface();
    if ([EAGLContext currentContext] == m_impl->m_context) {
        [EAGLContext setCurrentContext:nil];
    }
    m_impl->m_context = nil;
}
void OpenGLBackendiOS::create(const BackendOptions &options) {
    m_impl->m_context = (__bridge EAGLContext*)options.nativeContext;
    m_impl->m_options = options;
    if (options.alpha) {
        m_impl->colorFormat = kEAGLColorFormatRGBA8;
    }
    else {
        m_impl->colorFormat = kEAGLColorFormatRGB565;
    }
}

void OpenGLBackendiOS::makeCurrent() {
    [EAGLContext setCurrentContext:m_impl->m_context];
}
void OpenGLBackendiOS::bindScreenSurface() {
    makeCurrent();
    if (m_impl->m_options.antialiasing) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_msaaFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_msaaRenderbuffer);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultColorRenderbuffer);
    }
   //CHECK_GL_ERROR();
}

void OpenGLBackendiOS::swapBuffer() {
    makeCurrent();
    if (m_impl->m_options.antialiasing) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, m_impl->m_msaaFramebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, m_impl->m_defaultFramebuffer);
        glResolveMultisampleFramebufferAPPLE();
    }

    static bool checked_GL_EXT_discard_framebuffer = false;
    static bool has_GL_EXT_discard_framebuffer = false;
    if (!checked_GL_EXT_discard_framebuffer) {
        checked_GL_EXT_discard_framebuffer = true;
        const char *extensions = (const char *)glGetString(GL_EXTENSIONS);
        if (extensions) {
            has_GL_EXT_discard_framebuffer = strstr(extensions, "GL_EXT_discard_framebuffer") != NULL;
        }
    }
    if (has_GL_EXT_discard_framebuffer) {
        GLenum target = GL_FRAMEBUFFER;
        GLenum attachments[3];
        GLsizei numAttachments = 0;
        if (m_impl->m_options.antialiasing) {
            target = GL_READ_FRAMEBUFFER_APPLE;
            attachments[numAttachments++] = GL_COLOR_ATTACHMENT0;
        }
        if (m_impl->m_options.depth) {
            attachments[numAttachments++] = GL_DEPTH_ATTACHMENT;
        }
        if (m_impl->m_options.stencil) {
            attachments[numAttachments++] = GL_STENCIL_ATTACHMENT;
        }
        if (numAttachments > 0) {
            if (m_impl->m_options.antialiasing) {
                glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_msaaFramebuffer);
            }
            else {
                glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_defaultFramebuffer);
            }
            glDiscardFramebufferEXT(target, numAttachments, attachments);
        }
    }
    glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultColorRenderbuffer);
    [m_impl->m_context presentRenderbuffer:GL_RENDERBUFFER];
    //CHECK_GL_ERROR();
}
void OpenGLBackendiOS::createScreenSurface(void *nativeHandle) {
    m_impl->m_layer = (__bridge CAEAGLLayer*)nativeHandle;
    [EAGLContext setCurrentContext:m_impl->m_context];
    
    glGenFramebuffers(1, &m_impl->m_defaultFramebuffer);
    glGenRenderbuffers(1, &m_impl->m_defaultColorRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_defaultFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultColorRenderbuffer);
    
    if (![m_impl->m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:m_impl->m_layer]) {
        LOGE("Error: renderbufferStorage failed");
    }
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_impl->m_defaultColorRenderbuffer);
    
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    if (m_impl->m_options.antialiasing) {
        glGenFramebuffers(1, &m_impl->m_msaaFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_msaaFramebuffer);

        glGenRenderbuffers(1, &m_impl->m_msaaRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_msaaRenderbuffer);

        GLenum internalFormat = GL_RGBA8_OES;
        if ([kEAGLColorFormatRGB565 isEqualToString: m_impl->colorFormat]) {
            internalFormat = GL_RGB565;
        }

        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, internalFormat, width, height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_impl->m_msaaRenderbuffer);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("Error: Create multisample framebuffer failed: 0x%04x", status);
        }
    }
    
    if (m_impl->m_options.depth || m_impl->m_options.stencil) {
        glGenRenderbuffers(1, &m_impl->m_defaultDepthStencilRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_impl->m_defaultDepthStencilRenderbuffer);
        
        GLenum internalFormat;
        if (m_impl->m_options.depth > 0 && m_impl->m_options.stencil > 0) {
            internalFormat = GL_DEPTH24_STENCIL8_OES;
        } else if (m_impl->m_options.depth) {
            internalFormat = GL_DEPTH_COMPONENT16;//GL_DEPTH_COMPONENT24_OES;
        } else {
            internalFormat = GL_STENCIL_INDEX8;
        }
        
        if (m_impl->m_options.antialiasing) {
            glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, internalFormat, width, height);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
        }
        
        if (m_impl->m_options.depth) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_impl->m_defaultDepthStencilRenderbuffer);
        }
        if (m_impl->m_options.stencil) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_impl->m_defaultDepthStencilRenderbuffer);
        }
    }
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Error: Framebuffer incomplete: 0x%04x", status);
    }
    
    //CHECK_GL_ERROR();
    
    g_nRealMainFrameBuffer = m_impl->m_defaultFramebuffer;
}
void OpenGLBackendiOS::destroyScreenSurface() {
    makeCurrent();
    if (m_impl->m_defaultFramebuffer) {
        glDeleteFramebuffers(1, &m_impl->m_defaultFramebuffer);
        m_impl->m_defaultFramebuffer = 0;
    }
    if (m_impl->m_defaultColorRenderbuffer) {
        glDeleteRenderbuffers(1, &m_impl->m_defaultColorRenderbuffer);
        m_impl->m_defaultColorRenderbuffer = 0;
    }
    if (m_impl->m_defaultDepthStencilRenderbuffer) {
        glDeleteRenderbuffers(1, &m_impl->m_defaultDepthStencilRenderbuffer);
        m_impl->m_defaultDepthStencilRenderbuffer = 0;
    }
    if (m_impl->m_msaaRenderbuffer) {
        glDeleteRenderbuffers(1, &m_impl->m_msaaRenderbuffer);
        m_impl->m_msaaRenderbuffer = 0;
    }
    if (m_impl->m_msaaFramebuffer) {
        glDeleteFramebuffers(1, &m_impl->m_msaaFramebuffer);
        m_impl->m_msaaFramebuffer = 0;
    }
}

void OpenGLBackendiOS::onScreenSurfaceResize(int width, int height) {
    destroyScreenSurface();
    createScreenSurface((__bridge void*)m_impl->m_layer);
    makeCurrent();//todo
}
void OpenGLBackendiOS::resetRenderStates() {
}
}

#ifndef __GLUtils_H__
#define __GLUtils_H__

#include <string>
#include "GLObject.h"
#include "GLCapable.h"
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include "../../RenderInterface/IRenderVertexState.h"
#include "../../../RenderEngine/RenderEngine/WebGLEngine/GLCapable.h"

namespace laya
{
	GLuint getBytesPerPixel(GLenum type, GLenum format);
}
#endif
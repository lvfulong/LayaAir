#ifndef __GLUtils_H__
#define __GLUtils_H__

#include <string>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLObject.h"
#include "GLCapable.h"
#include <utils/Preprocessor.h>

namespace laya
{
	GLuint getBytesPerPixel(GLenum type, GLenum format);
}
#endif
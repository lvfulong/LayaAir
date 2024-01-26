#ifndef __GLUtils_H__
#define __GLUtils_H__

#include <string>
#include "GLObject.h"
#include "GLCapable.h"
#include <utils/Preprocessor.h>
#include <render/3D/design/renderInterface/IRenderVertexState.h>

namespace laya
{
	GLuint getBytesPerPixel(GLenum type, GLenum format);
}
#endif
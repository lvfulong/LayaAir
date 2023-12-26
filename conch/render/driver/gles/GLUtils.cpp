#include "GLUtils.h"

namespace laya
{
	GLuint getBytesPerPixel(GLenum type, GLenum format) 
	{
		int bytesPerComponent = 0;
		switch (type) 
		{
		case GL_UNSIGNED_BYTE:
			bytesPerComponent = 1;
			break;
		case GL_FLOAT:
			bytesPerComponent = 4;
			break;
		case 36193:// GL_HALF_FLOAT_OES:
			bytesPerComponent = 2;
			break;
		case GL_UNSIGNED_SHORT_5_6_5:
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_5_5_5_1:
			return 2;
		}

		switch (format) 
		{
		case GL_LUMINANCE:
		case GL_ALPHA:
			return 1 * bytesPerComponent;
		case GL_LUMINANCE_ALPHA:
			return 2 * bytesPerComponent;
		case GL_RGB:
			return 3 * bytesPerComponent;
		case GL_RGBA:
			return 4 * bytesPerComponent;
		}
		return 0;
	}
}
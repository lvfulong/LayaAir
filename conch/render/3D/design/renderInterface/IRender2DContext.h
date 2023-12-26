#ifndef __IRender2DContext_H__
#define __IRender2DContext_H__

namespace laya
{
	class IRender2DContext
	{
	public:
		virtual ~IRender2DContext() {}
		virtual void activeTexture(int textureID) = 0;
		virtual void bindTexture(int target, void* texture) = 0;
		virtual bool bindUseProgram(int webglProgram) = 0;
	};
}

#endif
#ifndef __JSUniformBufferObject_H__
#define __JSUniformBufferObject_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/3D/UniformBufferObject.h"

namespace laya
{
	class JSUniformBufferObject
	{
	public:
		static void exportJS(Context& context);
		JSUniformBufferObject();
		JSUniformBufferObject(JSValueAsParam engine, int glPointer);
		~JSUniformBufferObject();

	public:

		void setGLBuffer(JSValueAsParam pBuffer);

		int getNativeObjID();

	public:
		UniformBufferObject* m_pUniformBufferObject;
	};
}
#endif //__JSUniformBufferObject_H__
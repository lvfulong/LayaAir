#ifndef __JSIndexBuffer3D_H__
#define __JSIndexBuffer3D_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/3D/temp/IndexBuffer3D.h>

namespace laya
{
	class JSIndexBuffer3D
	{
	public:
		static void exportJS(Context& context);
		JSIndexBuffer3D();
		JSIndexBuffer3D(JSValueAsParam engine, int indexFormat,int nIndexCount,int bufferUsageType,bool bSelfCreateBuffer);
		~JSIndexBuffer3D();

	public:

		//bool bindBuffer();
		//void unbindBuffer();
		//void orphanStorage();
		//void setDataLength(int length);
		//void setData(JSValueAsParam buffer, int offset);
		//void setDataEx(JSValueAsParam buffer,int offset, int length);
		void setGLBuffer(JSValueAsParam pBuffer);

	public:
		IndexBuffer3D*	m_pIndexBuffer3D;
	};
}
#endif //__JSIndexBuffer3D_H__
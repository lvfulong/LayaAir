#ifndef __JSVertexBuffer3D_H__
#define __JSVertexBuffer3D_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include <render/3D/temp/VertexBuffer3D.h>

namespace laya
{
	class JSVertexBuffer3D
	{
	public:
		static void exportJS(Context& context);
		JSVertexBuffer3D();
		JSVertexBuffer3D(JSValueAsParam engine, int nByteLength, int bufferUsageType,bool bSelfCreateBuffer);
		~JSVertexBuffer3D();

	public:

		void setVertexDeclaration(JSValueAsParam buffer);
		//bool bindBuffer();
		//void unbindBuffer();
		//void setDataLength(int length);
		//void setData(JSValueAsParam buffer, int offset);
		//void setDataEx(JSValueAsParam buffer,int offset, int length);

		void setGLBuffer(JSValueAsParam pBuffer);
		bool getInstanceBuffer();
		void setInstanceBuffer(bool value);

	public:
		VertexBuffer3D* m_pVertexBuffer3D;
	};
}
#endif //__JSVertexBuffer3D_H__
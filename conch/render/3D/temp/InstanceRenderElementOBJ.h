#ifndef __InstanceRenderElementOBJ_H__
#define __InstanceRenderElementOBJ_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include "RenderGeometryElement.h"
#include <utils/JCSingletonList.h>
#include <render/driver/gles/GLShaderInstance.h>
#include "ShaderData.h"
//#include "RenderContext3D.h"
#include "ShaderInstance.h"
#include "RenderElement.h"
#include  <vector>

namespace laya
{
	class VertexBuffer3D;
	class WebGLEngine;
	class InstanceRenderElementOBJ : public RenderElement
	{
	public:
		InstanceRenderElementOBJ(WebGLEngine* pWebGLEngine);
		~InstanceRenderElementOBJ();

		void drawGeometry(ShaderInstance* shaderIns) override;

		void clear();

		int getDrawCount();

		void setDrawCount(int nValue);

		void addUpdateBuffer(VertexBuffer3D* vb, int length);

		void getUpdateData(int index, const char* pData);


	public:

		int																m_nDrawCount = 0;

		//int																m_nUpdateNums = 0;

		std::vector<std::pair<VertexBuffer3D*, int> >					m_vertexBuffer3D;

		//std::vector<int>												m_updateDataNum;
		std::vector<const char*>										m_updateData;
	};
}
#endif //__InstanceRenderElementOBJ_H__

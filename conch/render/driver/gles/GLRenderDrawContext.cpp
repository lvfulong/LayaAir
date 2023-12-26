#include "GLRenderDrawContext.h"
#include "GLEnum/WebGLExtension.h"
#ifdef __APPLE__
#include <OpenGLES/ES3/GL.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include "GLCapable.h"
#include <render/3D/temp/RenderGeometryElement.h>
#include "GLVertexState.h"

namespace laya
{
	GLRenderDrawContext::GLRenderDrawContext(WebGLEngine* engine): GLObject(engine)
	{
		if (!m_engine->isWebGL2())
		{
			m_angleInstancedArrays = (ANGLEInstancedArraysExt*)m_engine->getExtension(WebGLExtension::ANGLE_instanced_arrays);
		}
	}
	static GLenum getMeshTopology(MeshTopology mode) 
	{
		switch (mode)
		{
		case MeshTopology::Points:
			return GL_POINTS;
		case MeshTopology::Lines:
			return GL_LINES;
		case MeshTopology::LineLoop:
			return GL_LINE_LOOP;
		case MeshTopology::LineStrip:
			return GL_LINE_STRIP;
		case MeshTopology::Triangles:
			return GL_TRIANGLES;
		case MeshTopology::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case MeshTopology::TriangleFan:
			return GL_TRIANGLE_FAN;
		}
		return GL_POINTS;
	}

	static GLenum getIndexType(IndexFormat type)
	{
		switch (type) 
		{
		case IndexFormat::UInt8:
			return GL_UNSIGNED_BYTE;
		case IndexFormat::UInt16:
			return GL_UNSIGNED_SHORT;
		case IndexFormat::UInt32:
			return GL_UNSIGNED_INT;
		}
		return GL_UNSIGNED_BYTE;
	}
	void GLRenderDrawContext::drawElementsInstanced(MeshTopology mode, int count, IndexFormat type, int offset, int instanceCount)
	{
		GLenum glmode = getMeshTopology(mode);
		GLenum gltype = getIndexType(type);

		if (m_engine->isWebGL2()) 
			glDrawElementsInstanced(glmode, count, gltype, (const void*)offset, instanceCount);
		else
			m_angleInstancedArrays->drawElementsInstancedANGLE(glmode, count, gltype, (const void*)offset, instanceCount);

		m_engine->addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
		m_engine->addStatisticsInfo(RenderStatisticsInfo::InstanceDrawCall, 1);
		m_engine->addStatisticsInfo(RenderStatisticsInfo::Triangle, count / 3 * instanceCount);
	}
	void GLRenderDrawContext::drawArraysInstanced(MeshTopology mode, int first, int count, int instanceCount)
	{
		GLenum glmode = getMeshTopology(mode);

		if (m_engine->isWebGL2())
			glDrawArraysInstanced(glmode, first, count, instanceCount);
		else
			m_angleInstancedArrays->drawArraysInstancedANGLE(glmode, first, count, instanceCount);

		m_engine->addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
		m_engine->addStatisticsInfo(RenderStatisticsInfo::InstanceDrawCall, 1);
		//TODO glmode
		m_engine->addStatisticsInfo(RenderStatisticsInfo::Triangle, (count - 2) * instanceCount);
	}
	void GLRenderDrawContext::drawArrays(MeshTopology mode, int first, int count)
	{
		GLenum glmode = getMeshTopology(mode);
		glDrawArrays(glmode, first, count);

		m_engine->addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
		//TODO glmode
		m_engine->addStatisticsInfo(RenderStatisticsInfo::Triangle, (count - 2));
	}
	void GLRenderDrawContext::drawElements(MeshTopology mode, int count, IndexFormat type, int offset)
	{
		GLenum glmode = getMeshTopology(mode);
		GLenum gltype = getIndexType(type);
		glDrawElements(glmode, count, gltype, (const void*)offset);

		m_engine->addStatisticsInfo(RenderStatisticsInfo::DrawCall, 1);
		m_engine->addStatisticsInfo(RenderStatisticsInfo::Triangle, count / 3);
	}
	void GLRenderDrawContext::drawGeometryElement(RenderGeometryElement* geometryElement)
	{
		geometryElement->m_pBufferState3D->bindVertexArray();
		std::vector<int>& element = geometryElement->m_pDrawParams->m_vElements;
		int length = geometryElement->m_pDrawParams->getLength();
		switch (geometryElement->m_nDrawType) 
		{
		case DrawType::DrawArray:
			for (int i = 0; i<length; i += 2) 
			{
				drawArrays(geometryElement->m_nRenderMode, element[i], element[i + 1]);
			}
			break;
		case DrawType::DrawElement:
			for (int i = 0; i<length; i += 2) 
			{
				drawElements(geometryElement->m_nRenderMode, element[i + 1], geometryElement->m_nIndexFormat, element[i]);
			}
			break;
		case DrawType::DrawArrayInstance:
			for (int i = 0; i<length; i += 2)
			{
				drawArraysInstanced(geometryElement->m_nRenderMode, element[i], element[i + 1], geometryElement->m_nInstanceCount);
			}
			break;
		case DrawType::DrawElementInstance:
			for (int i = 0; i<length; i += 2)
			{
				drawElementsInstanced(geometryElement->m_nRenderMode, element[i + 1], geometryElement->m_nIndexFormat, element[i], geometryElement->m_nInstanceCount);
			}
			break;
		default:
			break;
		}
		geometryElement->m_pBufferState3D->unbindVertexArray();//��ű�������״̬ unbind
	}
}

#ifndef __GLVertexState_H__
#define __GLVertexState_H__

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
#include <render/3D/design/renderInterface/IRenderVertexState.h>

namespace laya
{
	class VertextBuffer;
	class IndexBuffer;
	class IndexBuffer2D;
	class VertexBuffer2D;

	class GLVertexState: public GLObject, public IRenderVertexState
	{
	public:
		GLVertexState(WebGLEngine* engine): GLObject(engine)
		{
			if (!engine->isWebGL2())
				m_vaoExt = (OESVertexArrayObjectExt*)engine->getExtension(WebGLExtension::OES_vertex_array_object);
			createVertexArray();
			m_angleInstancedArrays = (ANGLEInstancedArraysExt*)engine->getExtension(WebGLExtension::ANGLE_instanced_arrays);
		}
		~GLVertexState();
		void createVertexArray()
		{
			if (m_engine->isWebGL2())
				glGenVertexArrays(1, &m_vao);
			else
				m_vao = m_vaoExt->createVertexArrayOES();
		}

		void deleteVertexArray()
		{
			if (m_engine->isWebGL2())
				glDeleteVertexArrays(1, &m_vao);
			else
				m_vaoExt->deleteVertexArrayOES(m_vao);
		}

		void bindVertexArray() override
		{
			if (m_engine->m_GLBindVertexArray == this)
				return;
			if (m_engine->isWebGL2())
				glBindVertexArray(m_vao);
			else
				m_vaoExt->bindVertexArrayOES(m_vao);
			m_engine->m_GLBindVertexArray = this;
		}
		void unbindVertexArray() override
		{
			if (m_engine->isWebGL2())
				glBindVertexArray(0);
			else
				m_vaoExt->bindVertexArrayOES(0);
			m_engine->m_GLBindVertexArray = nullptr;
		}

		/*isVertexArray(): void {
			if (this._engine.isWebGL2)
				(<WebGL2RenderingContext>this._gl).isVertexArray(this._vao);
			else
				this._vaoExt.isVertexArrayOES(this._vao);
		}*/

		void applyVertexBuffer(const std::vector<VertexBuffer*>& vertexBuffers) override;

		void applyIndexBuffer(IndexBuffer* indexBuffer) override;

		void applyVertexBuffer(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffers) override;

		void applyIndexBuffer(IndexBuffer2D* indexBuffer) override;

		void vertexAttribDivisor(int index, int divisor)
		{
			if (m_engine->isWebGL2())
				glVertexAttribDivisor(index, divisor);
			else
				m_angleInstancedArrays->vertexAttribDivisorANGLE(index, divisor);
		}
		void destroy() override;
	private:
		ANGLEInstancedArraysExt* m_angleInstancedArrays;
		OESVertexArrayObjectExt* m_vaoExt;
		GLenum m_vao;
		//_vertexDeclaration: VertexDeclaration;
		//IndexBuffer* m_bindedIndexBuffer;
		//_vertexBuffers: VertexBuffer[];
	};
} 
#endif
#ifndef __IRenderVertexState_H__
#define __IRenderVertexState_H__

#include <vector>

namespace laya
{
	class IndexBuffer;
	class VertexBuffer;
	class IndexBuffer2D;
	class VertexBuffer2D;


	struct AttribInfo
	{
		uint32_t _type;
		uint32_t _size;
		uint32_t _off;
	};

	class IRenderVertexState
	{
	public:
		//_vertexDeclaration:VertexDeclaration;
		//_bindedIndexBuffer:IndexBuffer;
		//_vertexBuffers:VertexBuffer[];
		virtual ~IRenderVertexState() {}
		virtual void bindVertexArray() = 0;
		virtual void unbindVertexArray() = 0;
		virtual void applyVertexBuffer(const std::vector<VertexBuffer*>& vertexBuffers) = 0;
		virtual void applyIndexBuffer(IndexBuffer* indexBuffer) = 0;
		virtual void applyVertexBuffer(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffers) = 0;
		virtual void applyIndexBuffer(IndexBuffer2D* indexBuffer) = 0;
		virtual void destroy() = 0;
	};
}
#endif
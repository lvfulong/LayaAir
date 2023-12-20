#include "GLVertexState.h"
#include <cassert>
#include "../../LayaAir/3D/IndexBuffer.h"
#include "../../LayaAir/3D/VertexBuffer.h"
#include "../../LayaAir/2D/IndexBuffer2D.h"
#include "../../LayaAir/2D/VertexBuffer2D.h"

namespace laya
{

	void GLVertexState::applyVertexBuffer(const std::vector<VertexBuffer*>& vertexBuffers)
	{
        //this._vertexBuffers = vertexBuffer;
        if (m_engine->m_GLBindVertexArray == this) 
		{
			for (int i = 0, size = vertexBuffers.size(); i < size; i++)
			{
				VertexBuffer* element = vertexBuffers[i];
				int n = element->m_nVertextDeclarationNum;
                element->bind();
                for (int j = 0; j < n; j++) 
				{
					VertexDeclaration* verDec = &(element->m_pVertextDeclaration[j]);
                    glEnableVertexAttribArray(verDec->location);
                    glVertexAttribPointer(verDec->location, verDec->size, verDec->type, (verDec->normalize > 0) ? GL_TRUE : GL_FALSE, verDec->stride, ((const void*)verDec->offset));
                    if (element->m_bInstanceBuffer)
						glVertexAttribDivisor(verDec->location, 1);
                }
            }
        } 
		else
		{
            assert("BufferState: must call bind() function first.");
        }
    }

	void GLVertexState::applyIndexBuffer(IndexBuffer* indexBuffer)
	{
        //需要强制更新IndexBuffer
        
        if(indexBuffer == nullptr)
		{
            return;
        }
        if (m_engine->m_GLBindVertexArray == this)
		{
            //if (m_bindedIndexBuffer != indexBuffer) 
			{
                indexBuffer->bind();//TODO:可和vao合并bind
                //m_bindedIndexBuffer = indexBuffer;
            }
        } 
		else
		{
            assert("BufferState: must call bind() function first.");
        }
    }
	void GLVertexState::applyVertexBuffer(int stride, const std::vector<AttribInfo>& attribInfo, VertexBuffer2D* vertexBuffers)
	{
		//this._vertexBuffers = vertexBuffer;
		if (m_engine->m_GLBindVertexArray == this)
		{
			VertexBuffer2D* element = vertexBuffers;
			element->bind();
			int attribNum = attribInfo.size();
			for (int i = 0; i < attribNum; i++)
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, attribInfo[i]._size, attribInfo[i]._type, GL_FALSE, stride, (const void*)attribInfo[i]._off); //注意 normalize都设置为false了，想必没人要用这个功能把。
			}
		}
		else
		{
			assert("BufferState: must call bind() function first.");
		}
	}
	void GLVertexState::applyIndexBuffer(IndexBuffer2D* indexBuffer)
	{
		//需要强制更新IndexBuffer

		if (indexBuffer == nullptr)
		{
			return;
		}
		if (m_engine->m_GLBindVertexArray == this)
		{
			//if (m_bindedIndexBuffer2D != indexBuffer)
			{
				indexBuffer->bind();//TODO:可和vao合并bind
				//m_bindedIndexBuffer2D = indexBuffer;
			}
		}
		else
		{
			assert("BufferState: must call bind() function first.");
		}
	}


	void GLVertexState::destroy()
	{
		if (!m_destroyed)
		{
			deleteVertexArray();
			m_engine = nullptr;
			m_destroyed = true;
		}
    }
	GLVertexState::~GLVertexState()
	{
		destroy();
	}
} 
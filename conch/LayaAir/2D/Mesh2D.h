#ifndef __Mesh2D_H__
#define __Mesh2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include <utils/Preprocessor.h>

namespace laya
{
	class IndexBuffer2D;
	class VertexBuffer2D;
	class BufferStateBase;
	class WebGLEngine;


	class Mesh2D
	{	
	public:
		enum
		{
			BYTES_PER_INDEX = 2,
		};
		Mesh2D(WebGLEngine* engine, int stride, int vballoc, int iballoc, bool createIndexBuffer);
		virtual ~Mesh2D();
		void configVAO();
		void useMesh();
		virtual void recycle() {};
		void createQuadIB(int quadNum);
	public:
		int									m_stride = 0; //����ṹ��С��ÿ��mesh�Ķ���ṹ�ǹ̶��ġ�
		bool								m_canReuse = false;	//�����Ժ���ɾ�����ǻ��ա�
		int									m_vertNum = 0; //��ǰ�Ķ���ĸ���
		int									m_indexNum = 0; //ʵ��index ����������һ����������3��������ib�������ܳ���ʵ��ʹ�õ�������������Ҫһ��indexNum
		std::vector<AttribInfo>				m_attribInfo; //�������������Զ������顣
		VertexBuffer2D*						m_vb = nullptr; //vb��ib��������Ҫ���ⲿ�޸ģ�����public
		IndexBuffer2D*						m_ib = nullptr;
		bool								m_applied = false;	//�Ƿ��Ѿ����ø�webgl��
		BufferStateBase*					m_vao = nullptr;
		WebGLEngine*						m_pWebGLEngine;
		bool								m_createIndexBuffer = true;
	};
}
#endif //__Mesh2D_H__

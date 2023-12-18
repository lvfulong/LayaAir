#include "VertexBuffer.h"

namespace laya
{
	VertexBuffer::~VertexBuffer()
	{
		if (m_pVertextDeclaration)
		{
			delete[] m_pVertextDeclaration;
			m_pVertextDeclaration = NULL;
		}
	}
	void VertexBuffer::setVertexDeclaration(int* declaration, int intLength)
	{
		if (m_pVertextDeclaration != NULL)
		{
			delete[] m_pVertextDeclaration;
			m_pVertextDeclaration = NULL;
		}
		//TEST
		int n = m_nVertextDeclarationNum = intLength / sizeof(VertexDeclaration);
		m_pVertextDeclaration = new VertexDeclaration[n];
		int num = 0;
		for (int i = 0; i < n; i++)
		{
			m_pVertextDeclaration[i].location = declaration[num];
			num++;
			m_pVertextDeclaration[i].size = declaration[num];
			num++;
			m_pVertextDeclaration[i].type = declaration[num];
			num++;
			m_pVertextDeclaration[i].normalize = declaration[num];
			num++;
			m_pVertextDeclaration[i].stride = declaration[num];
			num++;
			m_pVertextDeclaration[i].offset = declaration[num];
			num++;
		}
	}
}
//------------------------------------------------------------------------------

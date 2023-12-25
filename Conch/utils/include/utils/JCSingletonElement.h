/**
@file			JCColor.h
@brief
@author			James
@version		1.0
@date			2013_12_19
*/

#ifndef __JCSingletonList_H__
#define __JCSingletonList_H__

#include <stdio.h>
#include <vector>

namespace laya
{
	template <class T>
	class JCSingletonList
	{
	public:
		JCSingletonList()
		{
			m_nLength = 0;
		}
		~JCSingletonList()
		{

		}
		inline void add(T element)
		{
			if (m_vElements.size() == m_nLength)
			{
				m_vElements.push_back(element);
			}
			else
			{
				m_vElements[m_nLength] = element;
			}
			m_nLength++;
		}
		inline void clearLength()
		{
			m_nLength = 0;
		}
		inline int getLength()
		{
			return m_nLength;
		}
	public:
		std::vector<T>	m_vElements;
	private:
		int				m_nLength;
	};
}

#endif //__JCSingletonList_H__

//-----------------------------END FILE--------------------------------
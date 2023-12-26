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
#include <cassert>

namespace laya
{
	template <class T>
	class JCSingletonList
	{
	public:
		JCSingletonList( bool bNeedDel )
		{
			m_bNeedDelete = bNeedDel;
			m_nLength = 0;
		}
		~JCSingletonList()
		{
			if (m_bNeedDelete)
			{
				for (int i = 0; i < m_nLength; i++)
				{
					T* pElement = &(m_vElements[i]);
					if (pElement)
					{
						delete pElement;
						pElement = NULL;
					}
				}
			}
			m_nLength = 0;
			m_vElements.clear();
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
		inline void resetLength()
		{
			assert(!m_bNeedDelete);
			m_nLength = 0;
		}
		inline void clear()
		{
			assert(!m_bNeedDelete);
			m_nLength = 0;
			m_vElements.clear();
		}
		inline int getLength()
		{
			return m_nLength;
		}
		inline void subLengthOne()
		{
			m_nLength--;
		}
	public:
		std::vector<T>	m_vElements;
	private:
		int				m_nLength;
		bool			m_bNeedDelete;
	};
}

#endif //__JCSingletonList_H__

//-----------------------------END FILE--------------------------------
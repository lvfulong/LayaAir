/**
@file			JCSimpleSingletonList.h
@brief
@author			James
@version		1.0
@date			2013_12_19
*/

#ifndef __JCSimpleSingletonList_H__
#define __JCSimpleSingletonList_H__

#include <stdio.h>
#include <vector>
#include "Log.h"
#include <cassert>

namespace laya
{
	template <class T>
	class JCSimpleSingletonList
	{
	public:
		JCSimpleSingletonList( bool bNeedDel )
		{
			m_bNeedDelete = bNeedDel;
			m_nLength = 0;
		}
		~JCSimpleSingletonList()
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
		void add(T element)
		{
			auto it = std::find(m_vElements.begin(), m_vElements.end(), element);

			if (it != m_vElements.end()) {
				return;
			}
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
		void remove(T element)
		{
			auto it = std::find(m_vElements.begin(), m_vElements.end(), element);

			if (it != m_vElements.end()) {

				int index = std::distance(m_vElements.begin(), it);
				if (index < m_nLength) {
					m_vElements[index] = m_vElements[m_nLength - 1];
					//m_vElements[m_nLength - 1] = null;//È¥µôÒýÓÃ
					m_nLength--;
				}
			}
		}
		void clear()
		{
		
			m_nLength = 0;
			m_vElements.clear();
		}
		
		inline int getLength()
		{
			return m_nLength;
		}
	public:
		std::vector<T>	m_vElements;
	private:
		bool								m_bNeedDelete;
		int									m_nLength{0};
	};
}

#endif //__JCSimpleSingletonList_H__

//-----------------------------END FILE--------------------------------

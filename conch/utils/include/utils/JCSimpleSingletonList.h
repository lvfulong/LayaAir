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
	class JCSingletonElement
	{
	public:
		JCSingletonElement()
		{
			m_nIndex = -1;
		}
		virtual ~JCSingletonElement()
		{

		}
		int getIndexInList()
		{
			return m_nIndex;
		}
		void setIndexInList(int  index)
		{
			m_nIndex = index;
		}
	public:
		int m_nIndex;
	};

	class JCSimpleSingletonList
	{
	public:
		JCSimpleSingletonList( bool bNeedDel )
		{
			//m_bNeedDelete = bNeedDel;
			m_nLength = 0;
		}
		~JCSimpleSingletonList()
		{
            clear();
			/*if (m_bNeedDelete)
			{
				for (int i = 0; i < m_nLength; i++)
				{
					delete m_vElements[i];
					m_vElements[i] = NULL;
				}
				m_vElements.clear();
				m_nLength = 0;
			}
			else
			{
				clear();
			}*/
		}
		void add(JCSingletonElement* element)
		{
			int nIndex = element->getIndexInList();
			if (nIndex != -1)
			{
				LOGE("JCSimpleSingletonList add: element + has  in  SingletonList.");
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
			element->setIndexInList(m_nLength++);
		}
		void remove(JCSingletonElement* element)
		{
			int index = element->getIndexInList();
            if (index == -1)
            {
               return;
            }
			m_nLength--;
			if (index != m_nLength)
			{
				JCSingletonElement* pEnd = m_vElements[m_nLength];
				m_vElements[index] = pEnd;
				pEnd->setIndexInList(index);
			}
			element->setIndexInList(-1);
		}
		void clear()
		{
			//assert(!m_bNeedDelete);
			for (int i = 0; i < m_nLength; i++)
			{
				m_vElements[i]->setIndexInList(-1);
			}
			m_nLength = 0;
			m_vElements.clear();
		}
		void resetLength()
		{
			for (int i = 0; i < m_nLength; i++)
			{
				m_vElements[i]->setIndexInList(-1);
			}
			m_nLength = 0;
		}
		inline int getLength()
		{
			return m_nLength;
		}
	public:
		std::vector<JCSingletonElement*>	m_vElements;
	private:
		bool								m_bNeedDelete;
		int									m_nLength;
	};
}

#endif //__JCSimpleSingletonList_H__

//-----------------------------END FILE--------------------------------

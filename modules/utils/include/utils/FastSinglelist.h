
#ifndef __FastSinglelist_H__
#define __FastSinglelist_H__

#include <cassert>
#include <stdio.h>
#include <vector>

namespace laya
{
template <class T> class FastSinglelist
{
  public:
    FastSinglelist(bool bNeedDel)
    {
        m_bNeedDelete = bNeedDel;
        m_nLength = 0;
    }
    ~FastSinglelist()
    {
        if (m_bNeedDelete)
        {
            for (int i = 0; i < m_nLength; i++)
            {
                T *pElement = &(m_vElements[i]);
                if (pElement)
                {
                    delete pElement;
                    pElement = NULL;
                }
            }
        }
        m_nLength = 0;
        elements.clear();
    }
    inline void add(T element)
    {
        if (elements.size() == m_nLength)
        {
            elements.push_back(element);
        }
        else
        {
            elements[m_nLength] = element;
        }
        m_nLength++;
    }
    inline void remove(T element)
    {
        int index = std::find(elements.begin(), elements.end(), element) - elements.begin();
        if (index < m_nLength)
        {
            elements[index] = elements[m_nLength - 1];
            elements[m_nLength - 1] = nullptr; // 去掉引用
            m_nLength--;
        }
    }
    inline void resetLength()
    {
        assert(!m_bNeedDelete);
        m_nLength = 0;
    }
    inline void setLength(int value)
    {
        assert(!m_bNeedDelete);
        m_nLength = value;
    }
    inline void clear()
    {
        assert(!m_bNeedDelete);
        m_nLength = 0;
        elements.clear();
    }
    inline int getLength()
    {
        return m_nLength;
    }
    inline int getLength() const
    {
        return m_nLength;
    }
    inline void subLengthOne()
    {
        m_nLength--;
    }

  public:
    std::vector<T> elements;

  private:
    int m_nLength;
    bool m_bNeedDelete;
};
} // namespace laya

#endif
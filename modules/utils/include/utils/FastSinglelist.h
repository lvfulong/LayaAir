
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
    inline void remove(T element)
    {
        int index = std::find(m_vElements.begin(), m_vElements.end(), element) - m_vElements.begin();
        if (index < m_nLength)
        {
            m_vElements[index] = m_vElements[m_nLength - 1];
            m_vElements[m_nLength - 1] = nullptr; // 去掉引用
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
        m_vElements.clear();
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
    std::vector<T> m_vElements;

  private:
    int m_nLength;
    bool m_bNeedDelete;
};
} // namespace laya

#endif
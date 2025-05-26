
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
                T *pElement = &(_elements[i]);
                if (pElement)
                {
                    delete pElement;
                    pElement = NULL;
                }
            }
        }
        m_nLength = 0;
        _elements.clear();
    }
    inline void add(T element)
    {
        if (_elements.size() == m_nLength)
        {
            _elements.push_back(element);
        }
        else
        {
            _elements[m_nLength] = element;
        }
        m_nLength++;
    }
    inline void remove(T element)
    {
        int index = std::find(_elements.begin(), _elements.end(), element) - _elements.begin();
        if (index < m_nLength)
        {
            _elements[index] = _elements[m_nLength - 1];
            _elements[m_nLength - 1] = nullptr; // 去掉引用
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
        _elements.clear();
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
    std::vector<T> _elements;

  private:
    int m_nLength;
    bool m_bNeedDelete;
};
} // namespace laya

#endif
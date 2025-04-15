/**
@file			JCSimpleSingletonList.h
@brief
@author			James
@version		1.0
@date			2013_12_19
*/

#ifndef __JCSimpleSingletonList_H__
#define __JCSimpleSingletonList_H__

#include "Log.h"
#include <cassert>
#include <functional>
#include <stdio.h>
#include <unordered_set>

namespace laya
{
template <class T> class JCSimpleSingletonList
{
  public:
    JCSimpleSingletonList(std::function<void(T)> deleteFunc = nullptr)
    {
        m_deleteFunc = deleteFunc;
    }
    ~JCSimpleSingletonList()
    {
        if (m_deleteFunc)
        {
            for (auto it = m_vElements.begin(); it != m_vElements.end(); ++it)
            {

                m_deleteFunc(*it);
            }
        }
        m_vElements.clear();
    }
    void add(T element)
    {
        m_vElements.insert(element);
    }
    void remove(T element)
    {
        m_vElements.erase(element);
    }
    void clear()
    {
        m_vElements.clear();
    }

    inline int getLength()
    {
        return m_vElements.size();
    }

  public:
    std::unordered_set<T> m_vElements;

  private:
    std::function<void(T)> m_deleteFunc;
};
} // namespace laya

#endif //__JCSimpleSingletonList_H__

//-----------------------------END FILE--------------------------------

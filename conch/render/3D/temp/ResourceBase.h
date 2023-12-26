#ifndef __ResourceBase_H__
#define __ResourceBase_H__

#include <unordered_map>

namespace laya
{
template <typename T> class ResourceManager;
template <typename T> class ResourceBase
{
  public:
    ResourceBase(ResourceManager<T> *pManager)
    {
        m_pManager = pManager;
        m_nID = m_pManager->getObjectID();
        m_pManager->addResource(m_nID, (T *)this);
    }
    virtual ~ResourceBase()
    {
        m_pManager->removeResource(m_nID);
    }
    int getID()
    {
        return m_nID;
    }

  protected:
    uint32_t m_nID;
    ResourceManager<T> *m_pManager;
};
template <typename T> class ResourceManager
{
  public:
    ResourceManager()
    {
    }
    ~ResourceManager()
    {
        m_ObjectMap.clear();
    }
    T *getResource(uint32_t id)
    {
        typename ObjectMapType::iterator it = m_ObjectMap.find(id);
        if (it != m_ObjectMap.end())
        {
            return it->second;
        }
        return nullptr;
    }
    void addResource(uint32_t id, T *object)
    {
        m_ObjectMap.insert(std::make_pair(id, object));
    }
    void removeResource(uint32_t id)
    {
        m_ObjectMap.erase(id);
    }
    friend class ResourceBase<T>;

  protected:
    uint32_t getObjectID()
    {
        return m_nObjectID++;
    }

  private:
    typedef std::unordered_map<uint32_t, T *> ObjectMapType;
    std::unordered_map<uint32_t, T *> m_ObjectMap;
    uint32_t m_nObjectID;
};
} // namespace laya
#endif
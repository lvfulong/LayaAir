#ifndef __ObjectBase_H__
#define __ObjectBase_H__

#include <unordered_map>

namespace laya
{
	template<typename T>
	class ObjectManager;
	template<typename T>
	class ObjectBase
	{
	public:
		ObjectBase(ObjectManager<T>* pManager)
		{
			m_pManager = pManager;
			m_nID = m_pManager->getObjectID();
			m_pManager->addObject(m_nID, (T*)this);
		}
		virtual ~ObjectBase()
		{
			m_pManager->removeObject(m_nID);
		}
		int getID() { return m_nID; }
	protected:
		int m_nID;
		ObjectManager<T>* m_pManager;
	};
	template<typename T>
	class ObjectManager
	{
	public:
		ObjectManager() {}
		~ObjectManager()
		{
			m_ObjectMap.clear();
		}
		T* getObject(int id)
		{
			typename ObjectMapType::iterator it = m_ObjectMap.find(id);
			if (it != m_ObjectMap.end())
			{
				return it->second;
			}
			return nullptr;
		}
		void addObject(int id, T* object)
		{
			m_ObjectMap.insert(std::make_pair(id, object));
		}
		void removeObject(int id)
		{
			m_ObjectMap.erase(id);
		}
		friend class ObjectBase<T>;
	protected:
		int getObjectID()
		{
			return m_nObjectID++;
		}
	private:
		typedef std::unordered_map<int, T*> ObjectMapType;
		std::unordered_map<int, T*> m_ObjectMap;
		int m_nObjectID;
	};
}
#endif
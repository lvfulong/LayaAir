#include "CommandUniformMap.h"

namespace laya
{
	std::unordered_map<std::string, CommandUniformMap*> CommandUniformMap::m_globalBlockMap;
	CommandUniformMap::CommandUniformMap()
	{
		
	}
	CommandUniformMap::~CommandUniformMap()
	{
	}
	CommandUniformMap* CommandUniformMap::createGlobalUniformMap(const char* blockName)
	{
		std::unordered_map<std::string, CommandUniformMap*>::iterator it = m_globalBlockMap.find(blockName);
		if (it != m_globalBlockMap.end())
		{
			return it->second;
		}
		CommandUniformMap* comMap = new CommandUniformMap(blockName);
		m_globalBlockMap[blockName] = comMap;
		return comMap;
	}

	bool CommandUniformMap::hasPtrID(int propertyID)
	{
		std::unordered_map<int, std::string>::iterator it = m_vData.find(propertyID);
		if (it != m_vData.end())
		{
			return true;
		}
		return false;
	}
}
//------------------------------------------------------------------------------

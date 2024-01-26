#ifndef __CommandUniformMap_H__
#define __CommandUniformMap_H__

#include <stdio.h>
#include <utils/Preprocessor.h>
#include <unordered_map>

#include <string>

namespace laya
{
	class CommandUniformMap
	{
	public:
		CommandUniformMap();
		~CommandUniformMap();


		static CommandUniformMap* createGlobalUniformMap(const char* blockName);

		CommandUniformMap(const char* stateName)
		{
			m_stateName = stateName;
		}

		bool hasPtrID(int propertyID);

		const std::unordered_map<int, std::string>& getMap()
		{
			return m_vData;
		}
		void addShaderUniform(int propertyID, const char* propertyKey) 
		{
			m_vData[propertyID] = propertyKey;
		}
	public:
		static std::unordered_map<std::string, CommandUniformMap*> m_globalBlockMap;
		std::string m_stateName;
		std::unordered_map<int, std::string> m_vData;
	};
}
#endif //__CommandUniformMap_H__

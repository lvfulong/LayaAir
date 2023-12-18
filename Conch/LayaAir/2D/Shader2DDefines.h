#ifndef __Shader2DDefines_H__
#define __Shader2DDefines_H__

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <stack>
#include "Buffer2D.h"
#include <string>

namespace laya
{
	class Shader2DDefines
	{	
	public:
		enum
		{
			TEXTURE2D = 0x01,
			PRIMITIVE = 0x04,
			FILTERGLOW = 0x08,
			FILTERBLUR = 0x10,
			FILTERCOLOR = 0x20,
			COLORADD = 0x40,

			WORLDMAT = 0x80,
			FILLTEXTURE = 0x100,
			SKINMESH = 0x200,
			MVP3D = 0x800,
			GAMMASPACE = 0x1000,

			NOOPTMASK = FILTERGLOW | FILTERBLUR | FILTERCOLOR | FILLTEXTURE,

		};
		Shader2DDefines() {}
		uint64_t add(const std::string& value);
		uint64_t add(uint64_t value);
		uint64_t remove(uint64_t value);
		static void reg(const std::string& name, uint32_t value);
		uint64_t getValue() const { return this->_value; }
		void setValue(uint64_t value) { this->_value = value; }
		const std::vector<std::string>& toNameDic() const;
		bool isDefine(uint64_t def);
		void clear();
	private:
		uint64_t _value = 0;
		static std::unordered_map<std::string, uint32_t> _name2int;;
		static std::unordered_map<uint32_t, std::string> _int2name;
		static std::unordered_map<uint32_t, std::vector<std::string> > _int2nameMap;
	};
}
#endif //__Shader2DDefines2D_H__
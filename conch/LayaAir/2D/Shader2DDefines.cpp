#include "Shader2DDefines.h"


namespace laya
{
	std::unordered_map<std::string, uint32_t> Shader2DDefines::_name2int = {
		{ "TEXTURE2D", Shader2DDefines::TEXTURE2D },
		{ "PRIMITIVE", Shader2DDefines::PRIMITIVE },
		{ "GLOW_FILTER", Shader2DDefines::FILTERGLOW },
		{ "BLUR_FILTER", Shader2DDefines::FILTERBLUR },
		{ "COLOR_FILTER", Shader2DDefines::FILTERCOLOR },
		{ "COLOR_ADD", Shader2DDefines::COLORADD },
		{ "WORLDMAT", Shader2DDefines::WORLDMAT },
		{ "FILLTEXTURE", Shader2DDefines::FILLTEXTURE },
		{ "MVP3D", Shader2DDefines::MVP3D },
		{ "GAMMASPACE", Shader2DDefines::GAMMASPACE },
	};
	std::unordered_map<uint32_t, std::string> Shader2DDefines::_int2name = {
		{ Shader2DDefines::TEXTURE2D, "TEXTURE2D" },
		{ Shader2DDefines::PRIMITIVE, "PRIMITIVE" },
		{ Shader2DDefines::FILTERGLOW, "GLOW_FILTER" },
		{ Shader2DDefines::FILTERBLUR, "BLUR_FILTER" },
		{ Shader2DDefines::FILTERCOLOR, "COLOR_FILTER" },
		{ Shader2DDefines::COLORADD, "COLOR_ADD" },
		{ Shader2DDefines::WORLDMAT, "WORLDMAT" },
		{ Shader2DDefines::FILLTEXTURE, "FILLTEXTURE" },
		{ Shader2DDefines::MVP3D, "MVP3D" },
		{ Shader2DDefines::GAMMASPACE, "GAMMASPACE" },
	};
	std::unordered_map<uint32_t, std::vector<std::string> > Shader2DDefines::_int2nameMap;

	uint64_t Shader2DDefines::add(const std::string& value)
	{
		this->_value |= this->_name2int[value];
		return this->_value;
	}
	uint64_t Shader2DDefines::add(uint64_t value)
	{
		this->_value |= value;
		return this->_value;
	}
	uint64_t Shader2DDefines::remove(uint64_t value)
	{
		this->_value &= (~value);
		return this->_value;
	}
	bool Shader2DDefines::isDefine(uint64_t def)
	{
		return (this->_value & def) == def;
	}
	void Shader2DDefines::reg(const std::string& name, uint32_t value)
	{
		Shader2DDefines::_name2int[name] = value;
		Shader2DDefines::_int2name[value] = name;
	}
	const std::vector<std::string>& Shader2DDefines::toNameDic() const
	{
		std::unordered_map<uint32_t, std::vector<std::string> >::iterator it = Shader2DDefines::_int2nameMap.find(this->_value);
		if (it != Shader2DDefines::_int2nameMap.end())
		{
			return it->second;
		}
		std::vector<std::string> o;
		uint64_t d = 1;
		for (int i = 0; i < 32; i++) 
		{
			d = 1 << i;
			if (d > this->_value) 
				break;
			if (this->_value & d)
			{
				std::string name = Shader2DDefines::_int2name[d];
				o.push_back(name);
			}
		}
		Shader2DDefines::_int2nameMap[this->_value] = o;
		return Shader2DDefines::_int2nameMap[this->_value];
	}
	//------------------------------------------------------------------------------
	void Shader2DDefines::clear()
	{
		this->_value = 0;
	}
}

//------------------------------------------------------------------------------

#ifndef __BlendMode_H__
#define __BlendMode_H__

#include <utils/Preprocessor.h>
#include <vector> 
#include <utils/Log.h>
#include <render/3D/design/renderEnum/BlendFactor.h>
#include <cstring>

namespace laya
{
	enum BlendMode
	{
		invalid = -1,
		normal = 0,
		add,
		multiply,
		screen,
		overlay,
		light,
		mask,
		destination_out,
		add_old,
		disable,
	};
	struct BlendModInfo
	{ 
		BlendMode type;
		BlendFactor source;
		BlendFactor destination;
		BlendFactor sourceTarget;
		BlendFactor destinationTarget;
	};
	extern std::vector<BlendModInfo> BlendModes;
	
	inline const char* blendModeToName(BlendMode value)
	{
		switch (value)
		{
		case BlendMode::normal:
			return "normal";
			break;
		case BlendMode::add:
			return "add";
			break;
		case BlendMode::multiply:
			return "multiply";
			break;
		case BlendMode::screen:
			return "screen";
			break;
		case BlendMode::overlay:
			return "overlay";
			break;
		case BlendMode::light:
			return "light";
			break;
		case BlendMode::mask:
			return "mask";
			break;
		case BlendMode::destination_out:
			return "destination-out";
			break;
		case BlendMode::add_old:
			return "add_old";
			break;
		default:
			LOGW("invalid BlendMode value");
			break;
		}
	}

	inline BlendMode nameToBlendMode(const char* name)
	{
		if (strcmp(name, "normal") == 0)
			return BlendMode::normal;
		else if (strcmp(name, "add") == 0 || strcmp(name, "lighter") == 0)
			return BlendMode::add;
		else if (strcmp(name, "multiply") == 0)
			return BlendMode::multiply;
		else if (strcmp(name, "screen") == 0)
			return BlendMode::screen;
		else if (strcmp(name, "overlay") == 0)
			return BlendMode::overlay;
		else if (strcmp(name, "light") == 0)
			return BlendMode::light;
		else if (strcmp(name, "mask") == 0)
			return BlendMode::mask;
		else if (strcmp(name, "destination-out") == 0)
			return BlendMode::destination_out;
		else if (strcmp(name, "add_old") == 0 || strcmp(name, "lighter_old") == 0)
			return BlendMode::add_old;
		else
			LOGW("invalid BlendMode name");
		return BlendMode::normal;
	}
}
#endif //__BlendMode_H__
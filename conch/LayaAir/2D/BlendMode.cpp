#include "BlendMode.h"

namespace laya
{
	std::vector<BlendModInfo> BlendModes = 
	{
		{ BlendMode::normal, BlendFactor::One, BlendFactor::OneMinusSourceAlpha,  BlendFactor::One, BlendFactor::OneMinusSourceAlpha },
		{ BlendMode::add,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One },
		{ BlendMode::multiply, BlendFactor::DestinationColor, BlendFactor::OneMinusSourceAlpha, BlendFactor::DestinationColor, BlendFactor::OneMinusSourceAlpha },
		{ BlendMode::screen,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One },
		{ BlendMode::overlay,  BlendFactor::One, BlendFactor::OneMinusSourceColor,  BlendFactor::One, BlendFactor::OneMinusSourceColor },
		{ BlendMode::light,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One,  BlendFactor::One },
		{ BlendMode::mask, BlendFactor::Zero, BlendFactor::SourceAlpha, BlendFactor::Zero, BlendFactor::SourceAlpha },
		{ BlendMode::destination_out, BlendFactor::Zero, BlendFactor::Zero, BlendFactor::Zero, BlendFactor::Zero },
		{ BlendMode::add_old,  BlendFactor::One, BlendFactor::DestinationAlpha,  BlendFactor::One, BlendFactor::DestinationAlpha },
		{ BlendMode::disable,  BlendFactor::One, BlendFactor::Zero,  BlendFactor::One, BlendFactor::Zero },
	};
}
//------------------------------------------------------------------------------

#ifndef __BlendFactor_H__
#define __BlendFactor_H__

namespace laya
{
	enum class BlendFactor
	{
		Zero,
		One,
		SourceColor,
		OneMinusSourceColor,
		DestinationColor,
		OneMinusDestinationColor,
		SourceAlpha,
		OneMinusSourceAlpha,
		DestinationAlpha,
		OneMinusDestinationAlpha,
		SourceAlphaSaturate,
		BlendColor,
		OneMinusBlendColor
	};
}
#endif
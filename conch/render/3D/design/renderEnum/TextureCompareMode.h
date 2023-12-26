#ifndef __TextureCompareMode_H__
#define __TextureCompareMode_H__

namespace laya
{
	enum class TextureCompareMode
	{
		None,
		LEQUAL,
		GEQUAL,
		LESS,
		GREATER,
		EQUAL,
		NOTEQUAL,
		ALWAYS,
		NEVER
	};
}
#endif
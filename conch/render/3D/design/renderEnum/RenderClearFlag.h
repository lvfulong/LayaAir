#ifndef __RenderClearFlag_H__
#define __RenderClearFlag_H__

namespace laya
{
	enum class RenderClearFlag
	{
		Nothing = 0,
		Color = 0x01,
		Depth = 0x02,
		Stencil = 0x04,
	};

	using RenderClearFlagBits = uint32_t;
}
#endif
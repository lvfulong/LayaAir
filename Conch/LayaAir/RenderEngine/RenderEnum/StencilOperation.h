#ifndef __StencilOperation_H__
#define __StencilOperation_H__

namespace laya
{
	enum class StencilOperation
	{
		Keep,
		Zero,
		Replace,
		IncrementSaturate,
		DecrementSaturate,
		Invert,
		IncrementWrap,
		DecrementWrap
	};
}
#endif
  
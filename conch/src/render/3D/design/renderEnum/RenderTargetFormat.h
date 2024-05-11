#ifndef __RenderTargetFormat_H__
#define __RenderTargetFormat_H__

#include "TextureFormat.h"

namespace laya
{
	enum class RenderTargetFormat
	{
		None = -1,
		R8G8B8 = (int)TextureFormat::R8G8B8,
		R8G8B8A8 = (int)TextureFormat::R8G8B8A8,
		R16G16B16A16 = (int)TextureFormat::R16G16B16A16,
		R32G32B32 = (int)TextureFormat::R32G32B32,
		R32G32B32A32 = (int)TextureFormat::R32G32B32A32,
		R16G16B16 = (int)TextureFormat::R16G16B16,

		DEPTH_16 = 35,
		STENCIL_8 = 36,
		DEPTHSTENCIL_24_8 = 37,
		DEPTH_32 = 38
	};
}
#endif
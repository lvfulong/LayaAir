#ifndef __RenderStatisticsInfo_H__
#define __RenderStatisticsInfo_H__

namespace laya
{
	enum class RenderStatisticsInfo
	{
		DrawCall,
		InstanceDrawCall,
		Triangle,
		UniformUpload,//TODO
		GPUMemory,
		TextureMemeory,
		RenderTextureMemory,
		BufferMemory,
	};
}
#endif
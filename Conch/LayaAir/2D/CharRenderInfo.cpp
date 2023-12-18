#include "CharRenderInfo.h"
#include "TextTexture.h"
#include <Utils/Log.h>

namespace laya
{
	int64_t RenderInfo::loopStTm = 0;
	int64_t RenderInfo::loopCount = 0;
	void CharRenderInfo::touch()
	{
		uint64_t curLoop = RenderInfo::loopCount;
		if (this->touchTick != curLoop)
		{// 这个保证每帧只调用一次
			std::shared_ptr<TextTexture> t = this->tex.lock();
			if (t)
			{
				t->touchRect(this, curLoop);
			}
		}
		this->touchTick = curLoop;
	}
	CharRenderInfo::~CharRenderInfo()
	{
		//LOGI("~CharRenderInfo");
	}
}

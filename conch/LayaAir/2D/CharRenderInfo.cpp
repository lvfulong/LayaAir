#include "CharRenderInfo.h"
#include "TextTexture.h"
#include <utils/Log.h>

namespace laya
{
	int64_t RenderInfo::loopStTm = 0;
	int64_t RenderInfo::loopCount = 0;
	void CharRenderInfo::touch()
	{
		uint64_t curLoop = RenderInfo::loopCount;
		if (this->touchTick != curLoop)
		{// �����֤ÿֻ֡����һ��
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

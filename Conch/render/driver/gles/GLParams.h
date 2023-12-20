#ifndef __GLParams_H__
#define __GLParams_H__

#include <unordered_map>
#include "../../RenderEnum/RenderParams.h"

namespace laya
{
	class WebGLEngine;
	class GLParams
	{
	public:
		GLParams(WebGLEngine* engine);
		int getParams(RenderParams params);
	private:
		void _initParams();
	private:
		WebGLEngine* m_engine;
		typedef std::unordered_map<RenderParams, int> ParamsDataMap;
		std::unordered_map<RenderParams, int> m_glParamsData;
	};
}
#endif
#ifndef __GLParams_H__
#define __GLParams_H__

#include <unordered_map>
#include <render/3D/design/renderEnum/RenderParams.h>
#include <utils/Preprocessor.h>
namespace laya
{
	class GLESEngine;
	class GLParams
	{
	public:
		GLParams(GLESEngine* engine);
		int getParams(RenderParams params);
	private:
		void _initParams();
	private:
		GLESEngine* m_engine;
		typedef std::unordered_map<RenderParams, GLfloat> ParamsDataMap;
		ParamsDataMap m_glParamsData;
	};
}
#endif
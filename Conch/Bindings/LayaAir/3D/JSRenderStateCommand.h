#ifndef __JSRenderStateCommand_H__
#define __JSRenderStateCommand_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/RenderEngine/RenderEnum/RenderStateType.h"

namespace laya
{
	class RenderStateCommand;
	class JSRenderStateCommand
	{
	public:
		static void exportJS(Context& context);

		JSRenderStateCommand();
		~JSRenderStateCommand();

		void addCMDInt1(int renderstate, int value0);
		void addCMDInt2(int renderstate, int value0, int value1);
		void addCMDInt3(int renderstate, int value0, int value1, int value2);
		void addCMDInt4(int renderstate, int value0, int value1, int value2, int value3);
		void clear();


	public:
		RenderStateCommand*		m_pRenderStateCommand;
	};
}
#endif //__JSRenderStateCommand_H__
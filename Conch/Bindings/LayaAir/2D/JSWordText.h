#ifndef __JSWordText_H__
#define __JSWordText_H__

#include <stdio.h>
#include <Binder/JSInterface.h>
#include "../../../LayaAir/2D/WordText.h"

namespace laya
{
	class JSWordText
	{
	public:
		static void exportJS(Context& context);

		JSWordText();
		~JSWordText();
		void cleanCache();
		bool getSplitRender();
		void setSplitRender(bool value);
		const char* getText();
		void setText(const char* text);
		int getID();
	public:
		WordText* m_wordText;
	};
}
#endif //__JSWordText_H__
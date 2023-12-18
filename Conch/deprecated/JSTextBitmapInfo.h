#ifndef __JSTextBitmapInfo_H__
#define __JSTextBitmapInfo_H__

#include <stdio.h>
#include <string>
#include <map>
#include <Binder/JSInterface.h>
#include <fontMgr/JCFontInfo.h>

namespace laya
{
    class JSTextBitmapInfo
    {
    public:
	    static void exportJS(Context& context);

        JSTextBitmapInfo();

	    ~JSTextBitmapInfo();

	    int getWidth();

	    int getHeight();

        JsValue getBitmapData();

        void setInfo(int w, int h, char* data);

    public:

	    int					m_nWidth;
	    int					m_nHeight;
        char*               m_pBitmapData;

    };
}
#endif 
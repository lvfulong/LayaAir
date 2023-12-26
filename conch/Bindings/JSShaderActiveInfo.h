#ifndef __JSShaderActiveInfo_H__
#define __JSShaderActiveInfo_H__


#include <stdio.h>
#include <string>
#include <map>
#include <binder/JSInterface.h>

namespace laya
{
    class JSShaderActiveInfo
    {
    public:
	    static void exportJS(Context& context);

        JSShaderActiveInfo();

	    ~JSShaderActiveInfo();

        int getType();

        int getSize();

        const char* getName();

    public:

        int         m_nType;
        int         m_nSize;
        std::string m_sName;

    };
}
#endif
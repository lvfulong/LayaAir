#ifndef __JSArrayBufferRef_H__
#define __JSArrayBufferRef_H__

#include <stdio.h>
#include <binder/JSInterface.h>


namespace laya 
{
    class JSArrayBufferRef
    {
    public:

	    static void exportJS(jsbind::Object& context);

        JSArrayBufferRef();

        ~JSArrayBufferRef();

        void callManagerRemoveArrayBuffer();

        int getID();

        bool getIsSyncToRender();

    public:

        int     m_nID;
        bool    m_bSyncToRender;
    };
}
#endif
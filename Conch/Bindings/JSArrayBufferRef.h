#ifndef __JSArrayBufferRef_H__
#define __JSArrayBufferRef_H__

#include <stdio.h>
#include <Binder/JSInterface.h>


namespace laya 
{
    class JSArrayBufferRef
    {
    public:

	    static void exportJS(Context& context);

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
#ifndef __JSConsole_H__
#define __JSConsole_H__

#include <stdio.h>
#include <Binder/JSInterface.h>


namespace laya 
{
    class JSConsole
    {
    public:
        static void exportJS(Context& context);
        
        static void log(int p_nType, const char* p_sBuffer);

    };
}
#endif
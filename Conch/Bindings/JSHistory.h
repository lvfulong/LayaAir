#ifndef __JSHistory_H__
#define __JSHistory_H__

#include <Binder/JSInterface.h>

namespace laya 
{

    class JSHistory
    {
    public:
        static void exportJS(Context& context);
    public:
        static int getLength();
        static void back();
        static void forward();
        static void go(int step);
        static void push(const char* strUrl);
    };

}
#endif 

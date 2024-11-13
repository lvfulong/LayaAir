#ifndef __JSNotify_H__
#define __JSNotify_H__


#include <stdio.h>
#include <string>
#include <jsbind/JSBind.h>


namespace laya 
{
    class JSNotify
    {
    public:

	    static void exportJS(jsbind::Object& context);

    
    public:
    
        //设置重复的消息
        //type  0是年  1是月  2是日 3是时  4是分  5是秒
        static void setRepeatNotify( int p_nID,int p_nStartTime,int p_nRepeatType,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
    
        //设置只提示一次的消息
        static void setOnceNotify( int p_nID,int p_nStartTime,const char* p_sTickerText,const char* p_sTitleText,const char* p_sDesc );
    
        //删除某一个消息和定时器
        static void deleteOnceNotify( int p_nID );
    
        //只删除全部消息，但是保留定时器
        static void deleteAllNotify();
    
    };
}

#endif
/**
@file			JCLayaThreadPool.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_12
*/

#include <utils/thread/JCLayaThreadPool.h>
#include <utils/Log.h>
#ifdef OS_WINDOWS
#include <windows.h>
#endif

namespace laya{

    thread_local long*	s_tls_curDataThread = nullptr;
	void ___datathread_onthreadstart(const char* threadname) {
#ifdef OS_WINDOWS
	{
		DWORD thid = GetCurrentThreadId();
		SetNameInternal(thid, threadname);
        LOGI("start thread:%s,%d", threadname, thid);
		//threadInfoLog("start thread:%s,%d", threadname, thid);
	}
#elif OS_ANDROID
	{
		//threadInfoLog("start thread:%s,%d", threadname, gettidv1());
	}
#elif OS_IOS
        pthread_setname_np(threadname);
#endif
	}
	void ___datathread_onthreadstop(const char* threadname) {
#ifdef OS_WINDOWS
	{
		DWORD thid = GetCurrentThreadId();
		LOGI("thread end:%s,%d", threadname, thid);
		//threadInfoLog("thread end:%s,%d", threadname, thid);
	}
#elif OS_ANDROID
	{
		LOGI("thread end:%s,%d", threadname, gettidv1());
		//threadInfoLog("thread end:%s,%ld", threadname, gettidv1());
	}
#endif
	}

}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

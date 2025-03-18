#ifndef __THREAD_H__
#define __THREAD_H__

#include <chrono>
#include <string>
#include <unordered_map>
#include <utils/MessageLoop.h>

namespace laya
{
void initializeCurrentThreadAsScriptThread();
bool isScriptThread();
void postToJS(std::function<void(void)> task);
} // namespace laya
#endif
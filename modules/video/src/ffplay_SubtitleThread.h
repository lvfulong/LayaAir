#ifndef __ffplay_SubtitleThread_H__
#define __ffplay_SubtitleThread_H__

#include "ffplay_Config.h"
namespace ffplay
{
struct VideoState;
int subtitle_thread(void *arg);
} // namespace ffplay
#endif
#ifndef __ffplay_ReadThread_H__
#define __ffplay_ReadThread_H__

#include "ffplay_Config.h"
namespace ffplay
{
struct VideoState;
int read_thread(void *arg);
} // namespace ffplay
#endif
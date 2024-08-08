#ifndef __ffplay_VideoThread_H__
#define __ffplay_VideoThread_H__

#include "libavcodec/packet.h"
#include "libavutil/fifo.h"
#include <SDL2/SDL_thread.h>
namespace ffplay
{
int video_thread(void *arg);
} // namespace ffplay
#endif
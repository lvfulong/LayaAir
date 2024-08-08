#ifndef __ffplay_ReadThread_H__
#define __ffplay_ReadThread_H__

#include "libavcodec/packet.h"
#include "libavutil/fifo.h"
#include <SDL2/SDL_thread.h>
namespace ffplay
{
struct VideoState;
int read_thread(void *arg);
} // namespace ffplay
#endif
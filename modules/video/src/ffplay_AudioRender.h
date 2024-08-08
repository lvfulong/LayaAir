#ifndef __ffplay_AudioRender_H__
#define __ffplay_AudioRender_H__

#include "ffplay_Config.h"
namespace ffplay
{
void sdl_audio_callback(void *opaque, Uint8 *stream, int len);
} // namespace ffplay
#endif
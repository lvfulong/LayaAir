#ifndef __ffplay_AudioThread_H__
#define __ffplay_AudioThread_H__

#include "ffplay_Config.h"
namespace ffplay
{
struct VideoState;
int configure_audio_filters(VideoState *is, const char *afilters, int force_output_format);
int audio_open(void *opaque, AVChannelLayout *wanted_channel_layout, int wanted_sample_rate,
               struct AudioParams *audio_hw_params);
int audio_thread(void *arg);
} // namespace ffplay
#endif
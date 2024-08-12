#ifndef __ffplay_VideoRender_H__
#define __ffplay_VideoRender_H__

#include "ffplay_Config.h"
namespace ffplay
{
struct VideoState;
void video_refresh(void *opaque, double *remaining_time);
int configure_video_filters(AVFilterGraph *graph, VideoState *is, const char *vfilters, AVFrame *frame);
} // namespace ffplay
#endif
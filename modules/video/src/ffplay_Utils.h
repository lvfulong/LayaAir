#ifndef __ffplay_Utils_H__
#define __ffplay_Utils_H__
#include "ffplay.h"
#include "ffplay_Config.h"
namespace ffplay
{
double get_rotation(const int32_t *displaymatrix);
int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec);
int decoder_start(Decoder *d, int (*fn)(void *), const char *thread_name, void *arg);
int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph, AVFilterContext *source_ctx,
                          AVFilterContext *sink_ctx);
int configure_audio_filters(VideoState *is, const char *afilters, int force_output_format);
void set_default_window_size(int width, int height, AVRational sar);
double get_master_clock(VideoState *is);
int get_master_sync_type(VideoState *is);
inline int compute_mod(int a, int b)
{
    return a < 0 ? a % b + b : a % b;
}
} // namespace ffplay
#endif
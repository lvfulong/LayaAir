#ifndef __ffplay_cmdutils_H__
#define __ffplay_cmdutils_H__
#include "libavformat/avformat.h"
namespace ffplay
{
int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec);

} // namespace ffplay
#endif
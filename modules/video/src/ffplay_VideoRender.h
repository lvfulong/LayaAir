#ifndef __ffplay_VideoRender_H__
#define __ffplay_VideoRender_H__

#include "ffplay_Config.h"
namespace ffplay
{
enum AVColorSpace sdl_supported_color_spaces[] = {
    AVCOL_SPC_BT709,
    AVCOL_SPC_BT470BG,
    AVCOL_SPC_SMPTE170M,
    AVCOL_SPC_UNSPECIFIED,
};
} // namespace ffplay
#endif
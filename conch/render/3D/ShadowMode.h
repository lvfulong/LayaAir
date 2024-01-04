#ifndef __ShadowMode_H__
#define __ShadowMode_H__

#include "render/driver/gles/LayaGL.h"

namespace laya
{
    enum class ShadowMode {
        None,
        Hard,
        SoftLow,
        SoftHigh
    };

    enum class ShadowCascadesMode
    {
        NoCascades,
        TwoCascades,
        FourCascades,
    };
} // namespace laya
#endif
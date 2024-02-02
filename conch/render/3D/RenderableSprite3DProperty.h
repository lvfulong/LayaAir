#ifndef __RenderableSprite3DProperty_H__
#define __RenderableSprite3DProperty_H__

#include "render/LayaGL.h"

namespace laya
{
class RenderableSprite3DProperty
{
  public:
    static uint32_t SAHDERDEFINE_LIGHTMAP;
    static uint32_t LIGHTMAP_DIRECTION;
    static uint32_t SHADERDEFINE_LIGHTMAP_DIRECTIONAL;
    static uint32_t REFLECTIONCUBE_PROBEPOSITION;
    static uint32_t REFLECTIONCUBE_PROBEBOXMAX;
    static uint32_t REFLECTIONCUBE_PROBEBOXMIN;
    static uint32_t AMBIENTCOLOR;
    static uint32_t IBLTEX;
    static uint32_t IBLROUGHNESSLEVEL;
    static uint32_t AMBIENTSH;
    static uint32_t REFLECTIONTEXTURE;
    static uint32_t REFLECTIONCUBE_HDR_PARAMS;
    static uint32_t AMBIENTSHAR;
    static uint32_t AMBIENTSHAG;
    static uint32_t AMBIENTSHAB;
    static uint32_t AMBIENTSHBR;
    static uint32_t AMBIENTSHBG;
    static uint32_t AMBIENTSHBB;
    static uint32_t AMBIENTSHC;
    static uint32_t AMBIENTINTENSITY;
    static uint32_t REFLECTIONINTENSITY;
    static uint32_t VOLUMETRICGI_PROBECOUNTS;
    static uint32_t VOLUMETRICGI_PROBESTEPS;
    static uint32_t VOLUMETRICGI_PROBESTARTPOS;
    static uint32_t VOLUMETRICGI_PROBEPARAMS;
    static uint32_t VOLUMETRICGI_IRRADIANCE;
    static uint32_t VOLUMETRICGI_DISTANCE;
    static void __init__();
};

} // namespace laya
#endif
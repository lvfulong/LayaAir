#ifndef __WebGLConfig_H__
#define __WebGLConfig_H__

#include <map>
#include <stdio.h>
#include <vector>
namespace laya
{
class WebGLConfig
{
  public:
    bool stencil;
    bool alpha;
    bool depth;
    bool antialias;
    bool failIfMajorPerformanceCaveat;
    bool premultipliedAlpha;
    bool preserveDrawingBuffer;
    // WebGLPowerPreference powerPreference;
};
} // namespace laya
#endif
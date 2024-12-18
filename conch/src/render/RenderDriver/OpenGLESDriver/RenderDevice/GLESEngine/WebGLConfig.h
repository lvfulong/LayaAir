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
    bool stencil{false};
    bool alpha{ false };
    bool depth{ false };
    bool antialias{ false };
    bool failIfMajorPerformanceCaveat{ false };
    bool premultipliedAlpha{ false };
    bool preserveDrawingBuffer{ false };
    bool enableUniformBufferObject{ true };
    bool _matUseUBO{ false };
};
} // namespace laya
#endif
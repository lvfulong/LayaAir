#ifndef __TYPES_H__
#define __TYPES_H__

#include <string>
namespace laya
{
enum class GraphicsAPI
{
    Invalid = 0,
    OpenGLES = 1,
    WebGL = 2,
};

std::string toString(GraphicsAPI api);
GraphicsAPI toGraphicsAPI(const std::string &str);
} // namespace laya
#endif

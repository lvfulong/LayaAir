#include <utils/Types.h>
#include <utils/JCCommonMethod.h>

namespace laya
{
std::string toString(GraphicsAPI api)
{
    switch (api)
    {
    case GraphicsAPI::Invalid:
        return "Invalid";
        break;
    case GraphicsAPI::OpenGLES:
        return "OpenGLES";
        break;
    case GraphicsAPI::WebGL:
        return "WebGL";
        break;
    default:
        return "Invalid";
        break;
    }

}
GraphicsAPI toGraphicsAPI(const std::string &str)
{
    if (compareStrings(str, "OpenGLES", false))
    {
        return GraphicsAPI::OpenGLES;
    }
    else if (compareStrings(str, "WebGL", false))
    {
        return GraphicsAPI::WebGL;
    }
    else
    {
        return GraphicsAPI::Invalid;
    }
}
} // namespace laya
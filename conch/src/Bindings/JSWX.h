
#ifndef __JSWX_H__
#define __JSWX_H__
#include <binder/JSInterface.h>
#include <utils/JCBuffer.h>
#include <map>
namespace laya
{

class JSWX
{

  public:
    static void exportJS(Context &context);

    static std::string createBufferURL(JSValueAsParam param);

    static void revokeBufferURL(const char *url);

  private:
    static std::map<std::string, JCBuffer*> m_bufferURLMap;
};
} // namespace laya

#endif

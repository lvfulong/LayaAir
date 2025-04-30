#ifndef __JSProfiler_H__
#define __JSProfiler_H__

#include <jsbind/JSBind.h>

namespace laya
{

class JSProfiler
{
  public:
    static void exportJS(jsbind::Object &context);

  public:
    static void ZoneStart(const std::string &strName);
    static void ZoneEnd();
};

} // namespace laya
#endif

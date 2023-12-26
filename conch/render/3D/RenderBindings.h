#ifndef __RenderBindings_H__
#define __RenderBindings_H__

#include <binder/JSInterface.h>

namespace laya
{
class RenderBindings
{
  public:
    static void exportJS(Context &context);
};

} // namespace laya
#endif
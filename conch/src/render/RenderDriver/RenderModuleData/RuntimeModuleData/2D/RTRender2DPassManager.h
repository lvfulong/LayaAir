#ifndef __RTRENDER2DPASSMANAGER_H__
#define __RTRENDER2DPASSMANAGER_H__
#include <core/math/Color.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <jsbind/JSBind.h>
#include <map>
#include <set>
#include <utils/FastSinglelist.h>
#include <vector>

namespace laya
{
class RTRender2DPass;
class GLESRenderContext2D;

class RTRender2DPassManager
{
  private:
    bool _modefy = false;
    std::vector<RTRender2DPass *> _passes;//lvtodo

    void _sortPassesByPriority();

  public:
    void removePass(RTRender2DPass *pass);
    void apply(GLESRenderContext2D *context);
    void clear();
    void addPass(RTRender2DPass *pass);
};
} // namespace laya
#endif // __RTRENDER2DPASSMANAGER_H__
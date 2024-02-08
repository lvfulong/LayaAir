#ifndef __RTDefineDatas_H__
#define __RTDefineDatas_H__
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTShaderDefine.h>
#include <vector>
namespace laya
{

class RTDefineDatas
{
  public:
    RTDefineDatas();
    ~RTDefineDatas();
    void _intersectionDefineDatas(RTDefineDatas *define);
    void add(RTShaderDefine define);
    void remove(RTShaderDefine define);
    bool has(RTShaderDefine define);
    void clear();
    void addDefineDatas(RTDefineDatas *defines);
    void removeDefineDatas(RTDefineDatas *defines);
    void cloneTo(RTDefineDatas *defines);
    RTDefineDatas *clone();
    void destroy();

  public:
    std::vector<uint32_t> _mask{};
    int32_t _length = 0;
};
} // namespace laya

#endif //__RTDefineDatas_H__
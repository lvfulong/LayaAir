#ifndef __RenderScene3D_H_
#define __RenderScene3D_H_

#include "core/math/Vector3.h"
#include "render/ShadowCullInfo.h"
#include <core/math/Bounds.h>
#include <core/math/Matrix4x4.h>
#include <unordered_map>

namespace laya
{

class GLESBaseRenderNode;
/*class CameraNode
{
  public:
    CameraNode();
    ~CameraNode();

  public:
    JSTransform *transform;
    float farplane;
    float nearplane;
    Matrix4x4 projectionViewMatrix;
    float fieldOfView;
    float aspectRatio;

  private:
};*/

class IDirectLightShadowRP
{
  public:
    IDirectLightShadowRP(){};
    virtual ~IDirectLightShadowRP(){};
    // virtual void update(RenderContext3D* context) = 0;
    // virtual void render(RenderContext3D* context, std::vector<GLESBaseRenderNode*>& renderNodeList, uint32_t count) =
    // 0;
  public:
    // light : IDirectLightData;
    // camera: ICameraNodeData;
    // destTarget: InternalRenderTarget;
};

/*class IRender3DProcess
{
public:
    virtual void renderCameraForwardPass(RenderContext3D* context, IForwardAddRP* passInfo, std::vector<BaseRenderNode*>
renderNodeList, uint32_t count) = 0;
};*/
} // namespace laya
#endif
#include "GLESDirectLightShadowCastRP.h"

namespace laya {

    //GLESDirectLightShadowCastRP
    GLESDirectLightShadowCastRP::GLESDirectLightShadowCastRP() {

    }

    GLESDirectLightShadowCastRP::~GLESDirectLightShadowCastRP() {

    }

    void GLESDirectLightShadowCastRP::update(RenderContext3D* context) {
        //根据cameraInfo的数据和mode还有directlight的数据,确认裁剪数据,确认shaderData的值
    }
    void GLESDirectLightShadowCastRP::render(RenderContext3D* context, std::vector<BaseRenderNode*> renderNodeList, uint32_t count) {
        //for循环  裁剪完 更新渲染数据，再渲染
    }

    void GLESDirectLightShadowCastRP::applyRenderData(uint32_t sceneShaderData, uint32_t cameraShaderData) {
        //将渲染结果 写入scene和camera的shaderData中
    }

    void GLESDirectLightShadowCastRP::set_lightUp(const Vector3& value) {
    
    }
    void GLESDirectLightShadowCastRP::set_lightSide(const Vector3&  value) {
    
    }
    void GLESDirectLightShadowCastRP::set_lightForward(const Vector3&  value) {
    
    }
    
    void GLESDirectLightShadowCastRP::set_shadowCascadeMode(ShadowCascadesMode value) {
    
    }
    void GLESDirectLightShadowCastRP::set_cameraInfo(CameraInfo value) {
        //TODO
    }
    void GLESDirectLightShadowCastRP::set_destTarget(uint32_t value) {
        //TODO
    }
}
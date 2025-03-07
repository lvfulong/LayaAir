#ifndef __GLESShaderInstance_H__
#define __GLESShaderInstance_H__

#include "render/3D/temp/CommandEncoder.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include <stdio.h>
#include <utils/Preprocessor.h>
namespace laya
{
class GLStateMap;
class ShaderVariable;
class RenderState;
class RTShaderPass;
class GLESShaderInstance //: public ResourceBase<GLESShaderInstance>
{
public:
    static std::vector<std::string>* _preDrawUnifromMaps;
  public:
    GLESShaderInstance(bool is2D, const char *vs, const char *ps,
                       const std::unordered_map<std::string, int32_t> &attributeMap, RTShaderPass *shaderPass);
    ~GLESShaderInstance();
    bool bind();
    void destroy();
    void _disposeResource();
    void uploadUniforms(CommandEncoder *shaderUniform, GLESShaderData *shaderDatas, bool uploadUnTexture);
    void uploadRenderStateFrontFace(GLESShaderData *shaderDatas, bool isTarget, bool invertFront);
    void uploadRenderStateBlendDepth(GLESShaderData *shaderDatas);
    void uploadRenderStateBlendDepthByMaterial(GLESShaderData *shaderDatas);
    void uploadRenderStateBlendDepthByShader(GLESShaderData *shaderDatas);
    bool complete();
  	
  private:
    void _create3D();
    void _create2D();
    bool hasSpritePtrID(int32_t dataOffset);
    std::string _hasAdditionShaderData(int dataOffset);
  public: 
    CommandEncoder m_sceneUniformParamsMap;
    CommandEncoder m_cameraUniformParamsMap;
    CommandEncoder m_spriteUniformParamsMap;
    CommandEncoder m_materialUniformParamsMap;
    CommandEncoder m_sprite2DUniformParamsMap;
    GLShaderInstance *m_GLShaderInstance;
    uint32_t m_uploadMark = 0;
    GLESShaderData *m_uploadScene = nullptr;
    GLESShaderData *m_uploadRender = nullptr;
    GLESShaderData *m_uploadCameraShaderValue = nullptr;
    GLESShaderData *m_uploadMaterial = nullptr;
    GLESEngine *m_pGLESEngine = nullptr;
    RTShaderPass *_shaderPass = nullptr;
    std::unordered_map<int, ShaderVariable> _cacheShaerVariable{};
    std::unordered_map<std::string, CommandEncoder> _additionUniformParamsMaps{};
    std::unordered_map<std::string, GLESShaderData*> _additionShaderData{};
};
} // namespace laya
#endif //__GLESShaderInstance_H__

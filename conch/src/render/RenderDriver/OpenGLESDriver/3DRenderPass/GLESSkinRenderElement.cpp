#include "GLESSkinRenderElement.h"
#include <Bindings/LayaAir/3D/JSRTTransform.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h>
#include "render/Property.h"

namespace laya
{
GLESSkinRenderElement::GLESSkinRenderElement()
{
}
GLESSkinRenderElement::~GLESSkinRenderElement()
{
}

void GLESSkinRenderElement::drawGeometry(GLESShaderInstance* shaderIns)
{

    std::vector<int>& element = geometry->m_pDrawParams->m_vElements;
    if (m_vSkinData.empty())
        return;
    geometry->_bufferState->bind();

   auto iter = shaderIns->_cacheShaerVariable.find(SkinnedMeshSprite3DProperty::BONES);
   ShaderVariable* shaderVariable = nullptr;
   if (iter != shaderIns->_cacheShaerVariable.end()) {
       shaderVariable = &(iter->second);
   }
   if (shaderVariable == nullptr) {
       std::vector<ShaderVariable*>& idata = shaderIns->m_spriteUniformParamsMap.m_idata;
       for (int i = 0, n = idata.size(); i < n; i++) {
           if (idata[i]->dataOffset == SkinnedMeshSprite3DProperty::BONES) {
               shaderVariable = idata[i];
               shaderIns->_cacheShaerVariable[SkinnedMeshSprite3DProperty::BONES] = *shaderVariable;
               break;
           }
       }
   }

    for (int j = 0, m = geometry->m_pDrawParams->getLength() / 2; j < m; j++)
    {
        std::pair<char*, int>& subSkinnedDatas = m_vSkinData[j];
        /*shaderIns->uploadCustomUniforms(SkinnedMeshSprite3DProperty::BONES, subSkinnedDatas.first,
            subSkinnedDatas.second);*/

        static BufferDataInfo tempData;
        tempData.m_data = (uint8_t*)subSkinnedDatas.first;
        tempData.m_lengthInBytes = subSkinnedDatas.second;
        static std::any tempAny;
        tempAny = tempData;
        LayaGL::m_pWebglEngine->uploadOneUniforms(shaderIns->m_GLShaderInstance, shaderVariable, tempAny);

        int offset = j * 2;
        LayaGL::m_pWebglEngine->getDrawContext()->drawElements(geometry->_glmode, element[offset + 1],
            geometry->_glindexFormat, element[offset]);
    }
}
void GLESSkinRenderElement::setSkinnedData(std::vector<jsbind::ArrayBuffer> pData)
{
    m_vSkinData.clear();
    for (int i = 0, size = pData.size(); i < size; i++)
    {
        m_vSkinData.push_back(std::make_pair(reinterpret_cast<char*>(pData[i].getData()), pData[i].getByteLength()));
    }
}
} // namespace laya

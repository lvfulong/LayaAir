#include "GLESSkinRenderElement.h"
#include <Bindings/LayaAir/3D/JSRTTransform.h>
#include <render/Property.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESBufferState.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine/GLRenderDrawContext.h>

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

    for (int j = 0, m = geometry->m_pDrawParams->getLength() / 2; j < m; j++)
    {
        std::pair<char*, int>& subSkinnedDatas = m_vSkinData[j];
        shaderIns->uploadCustomUniforms(SkinnedMeshSprite3DProperty::BONES, subSkinnedDatas.first,
            subSkinnedDatas.second);
        int offset = j * 2;
        LayaGL::m_pWebglEngine->getDrawContext()->drawElements(geometry->_glmode, element[offset + 1],
            geometry->_glindexFormat, element[offset]);
    }
}
void GLESSkinRenderElement::setSkinnedData(JSValueAsParam pData)
{

    std::vector<JsValue> vecDatas;
    __JsArray<JsValue>::FromJsArray(pData, vecDatas);
    m_vSkinData.clear();
    for (int i = 0, size = vecDatas.size(); i < size; i++)
    {
        char *pArrayBufferPtr = NULL;
        int nABLen = 0;
        bool bIsArrayBuffer = extractJSAB(vecDatas[i], pArrayBufferPtr, nABLen);
        m_vSkinData.push_back(std::make_pair(pArrayBufferPtr, nABLen));
    }
}
} // namespace laya

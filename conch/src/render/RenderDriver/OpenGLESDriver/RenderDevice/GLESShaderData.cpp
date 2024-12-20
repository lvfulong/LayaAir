#include "GLESShaderData.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include <cassert>
#include <utils/Log.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESUniformBufferBase.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESSubUniformBuffer.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESCommandUniformMap.h"
#include <functional>
#include <string>
#include <any>

namespace laya
{
GLESShaderData::GLESShaderData(RTDefineDatas* defineDatas) //: ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
{
    _defineDatas = defineDatas;
}

GLESShaderData::~GLESShaderData()
{
    destroy();
}

void GLESShaderData::createUniformBuffer(const std::string &name, GLESCommandUniformMap* uniformMap) {
    if (!LayaGL::m_pWebglEngine->_config.enableUniformBufferObject|| _uniformBuffers.find(name)!= _uniformBuffers.end() ){
        return;
    }
    GLESUniformBuffer* uboBuffer = new GLESUniformBuffer(name);
    for (int i = 0, n = uniformMap->_uniformArray.size(); i < n; i++) {
        UniformProperty* a = uniformMap->_uniformArray[i];
        uboBuffer->addUniform(a->id, a->uniformtype, a->arrayLength);
    }
    uboBuffer->create();
    _uniformBuffers[name] = uboBuffer;
    int id = LayaGL::m_pWebglEngine->propertyNameToID(name.c_str());
    m_data[id] = new uniformDataShell( uboBuffer);
    for (auto it = uniformMap->_idata.begin(); it != uniformMap->_idata.end(); ++it) {
        UniformProperty* a = &it->second;
        if (m_data.find(a->id) != m_data.end()) {
            uboBuffer->setUniformData(a->id, a->uniformtype, m_data[a->id]);
        }
        _uniformBuffersPropertyMap[a->id] = uboBuffer;
    }
   
   
}

void GLESShaderData::updateUBOBuffer(const std::string &name) {
    if (!LayaGL::m_pWebglEngine->_config.enableUniformBufferObject&& _uniformBuffers.find(name) != _uniformBuffers.end()) {
        return;
    }
    GLESUniformBuffer* uboBuffer = _uniformBuffers[name];
    for (auto it = _updateCacheArray.begin(); it != _updateCacheArray.end(); ++it) {
        it->second(this, it->first);
    }
    _updateCacheArray.clear();
    if (uboBuffer->needUpload) {
        uboBuffer->upload();
    }
}

GLESSubUniformBuffer* GLESShaderData::createSubUniformBuffer(const std::string &name, std::vector<UniformProperty>& uniformMap){
    if (!LayaGL::m_pWebglEngine->_config.enableUniformBufferObject) {
        return nullptr;
    }
    if (_subUniformBuffers.find(name) != _subUniformBuffers.end()) {
        GLESSubUniformBuffer* subBuffer = _subUniformBuffers[name];
        for (auto it = _updateCacheArray.begin(); it != _updateCacheArray.end(); ++it) {
            it->second(this, it->first);
        }
        _updateCacheArray.clear();
        return subBuffer;
    }

    //create SubUniformBuffer
    GLESUniformBufferManager* mgr = LayaGL::m_pWebglEngine->bufferMgr;

    GLESSubUniformBuffer* subBuffer = new GLESSubUniformBuffer(name, uniformMap, mgr, this);
    subBuffer->notifyGPUBufferChange();
    _subUniformBuffers[name] = subBuffer;
    int id = LayaGL::m_pWebglEngine->propertyNameToID(name.c_str());
    m_data[id] = new uniformDataShell(subBuffer);
    for (int i = 0,n = uniformMap.size(); i<n; i++) {
        if (i == 2) {
            int a = 10;
        }
        UniformProperty* a = &uniformMap[i];
        if (m_data.find(a->id) != m_data.end()) {
            subBuffer->setUniformData(a->id, a->uniformtype, m_data[a->id]);
        }
        _uniformBuffersPropertyMap[a->id] = subBuffer;
    }
    return subBuffer;

}

void GLESShaderData::clearData()
{
    _updateCacheArray.clear();
    _uniformBuffersPropertyMap.clear();
    for (auto it = _uniformBuffers.begin(); it != _uniformBuffers.end(); ++it) {
        //TODO
        it->second->destroy();
    }
    _uniformBuffers.clear();

    for (auto it = _subUniformBuffers.begin(); it != _subUniformBuffers.end(); ++it) {
        //TODO
        it->second->destroy();
    }
    _subUniformBuffers.clear();

    m_data.clear();
    m_gammaColorMap.clear();

    _defineDatas->clear();
}

void GLESShaderData::destroy()
{
    isDestroy = true;
    clearData();
    _defineDatas->destroy();
    _defineDatas = nullptr;
}

void GLESShaderData::addDefine(RTShaderDefine define)
{
    _defineDatas->add(define);
}

void GLESShaderData::addDefines(RTDefineDatas *defines)
{
    _defineDatas->addDefineDatas(defines);
}

void GLESShaderData::removeDefine(RTShaderDefine define)
{
    _defineDatas->remove(define);
}

bool GLESShaderData::hasDefine(RTShaderDefine define)
{
   return _defineDatas->has(define);
}

void GLESShaderData::clearDefine()
{
    _defineDatas->clear();
}

void GLESShaderData::setBool(int32_t index, bool value)
{
    m_data[index] = (int32_t)value;
}

bool* GLESShaderData::getBool(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        static bool tmp;
        tmp = std::any_cast<int32_t>(m_data[index]) > 0 ? true : false;
        return &tmp;
    }
    else
    {
        return nullptr;
    }

}

void GLESShaderData::setInt(int32_t index, int32_t value)
{
    m_data[index] = value;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setInt(index, std::any_cast<int32_t>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

int32_t* GLESShaderData::getInt(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<int32_t&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setNumber(int32_t index, float value)
{
    m_data[index] = value;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setFloat(index, std::any_cast<float>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

float* GLESShaderData::getNumber(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<float&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setVector2(int32_t index, const Vector2 &value)
{
    m_data[index] = value;

    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setVector2(index, std::any_cast<Vector2>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Vector2* GLESShaderData::getVector2(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<Vector2&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setVector(int32_t index, const Vector4 &value)
{
    this->m_data[index] = value;

    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setVector4(index, std::any_cast<Vector4>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Vector4* GLESShaderData::getVector(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<Vector4&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setVector3(int32_t index, const Vector3 &value)
{
    this->m_data[index] = value;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setVector3(index, std::any_cast<Vector3>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Vector3* GLESShaderData::getVector3(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<Vector3&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setColor(int32_t index, const Color &value)
{
    if (m_data.find(index) != m_data.end())
    {
        m_gammaColorMap[index] = value;
        Vector4 &linearColor = std::any_cast<Vector4 &>(m_data[index]);
        linearColor.x = Color::gammaToLinearSpace(value.r);
        linearColor.y = Color::gammaToLinearSpace(value.g);
        linearColor.z = Color::gammaToLinearSpace(value.b);
        linearColor.w = value.a;
    }
    else
    {
        Vector4 linearColor = Vector4();
        linearColor.x = Color::gammaToLinearSpace(value.r);
        linearColor.y = Color::gammaToLinearSpace(value.g);
        linearColor.z = Color::gammaToLinearSpace(value.b);
        linearColor.w = value.a;
        m_data[index] = linearColor;
        m_gammaColorMap[index] = Color(value);
    }

    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setVector4(index, std::any_cast<Vector4>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Color* GLESShaderData::getColor(int32_t index)
{
    if (m_gammaColorMap.find(index) == m_gammaColorMap.end())
    {
        return nullptr;
    }
    return &m_gammaColorMap[index];
}

void GLESShaderData::setMatrix3x3(int32_t index, const Matrix3x3 &value)
{
    m_data[index] = value;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setMatrix3x3(index, std::any_cast<Matrix3x3>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Matrix3x3* GLESShaderData::getMatrix3x3(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<Matrix3x3&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setMatrix4x4(int32_t index, const Matrix4x4 &value)
{
    m_data[index] = value;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setMatrix4x4(index, std::any_cast<Matrix4x4>(data->m_data[index]));
            }
            };
        _updateCacheArray[index] = fun;
    }
}

Matrix4x4* GLESShaderData::getMatrix4x4(int32_t index)
{
    if (m_data.find(index) != m_data.end())
    {
        return &std::any_cast<Matrix4x4&>(m_data[index]);
    }
    else
    {
        return nullptr;
    }
}

void GLESShaderData::setBuffer(int32_t index, uint8_t *data, uint32_t lengthInBytes)
{
    BufferDataInfo info;
    info.m_data = data;
    info.m_lengthInBytes = lengthInBytes;
    m_data[index] = info;
    if (_needCacheData) {
        std::function<void(GLESShaderData*, int32_t)> fun = [](GLESShaderData* data, int32_t index) {
            if (data->_uniformBuffersPropertyMap.find(index) != data->_uniformBuffersPropertyMap.end()) {
                data->_uniformBuffersPropertyMap[index]->setBuffer(index, std::any_cast<BufferDataInfo>(data->m_data[index]).m_data);
            }
            };
        _updateCacheArray[index] = fun;
    }
}

void GLESShaderData::setBuffer(int32_t index, BufferDataInfo& data)
{
    m_data[index] = data;
}

const BufferDataInfo &GLESShaderData::getBuffer(int32_t index)
{
    return std::any_cast<const BufferDataInfo &>(m_data[index]);
}

void GLESShaderData::setInternalTexture(int32_t index, GLESInternalTex *value)
{
    if (value != nullptr)
    {
        bool data = GLESEngine::_texGammaDefine.find(index) != GLESEngine::_texGammaDefine.end();
        if (data && value->gammaCorrection > 1)
        {
            addDefine(GLESEngine::_texGammaDefine[index]);
        }
        else
        {
            if (data)
                removeDefine(GLESEngine::_texGammaDefine[index]);
        }
    }
    m_data[index] = value;
}

GLESInternalTex *GLESShaderData::getInternalTexture(int32_t index)
{
    return std::any_cast<GLESInternalTex *>(m_data[index]);
}

void GLESShaderData::cloneTo(GLESShaderData *destObject)
{
    this->_defineDatas->cloneTo(destObject->_defineDatas);
    destObject->m_data = this->m_data;
    destObject->m_gammaColorMap = this->m_gammaColorMap;
}
} // namespace laya

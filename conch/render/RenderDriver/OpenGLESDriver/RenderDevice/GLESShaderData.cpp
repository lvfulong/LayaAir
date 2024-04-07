#include "GLESShaderData.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include <cassert>
#include <utils/Log.h>

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

void GLESShaderData::destroy()
{
    isDestroy = true;
    m_data.clear();
}

/*RTDefineDatas* GLESShaderData::getOwnerDefineData()
{
    return _defineDatas;
}

JsValue GLESShaderData::getOwnerDefineDataJS()
{
    if (m_pJSDefineDatas.isEmpty())
    {
        m_pJSDefineDatas.reset(Converter<RTDefineDatas *>::ToJs(_defineDatas, false));
        return m_pJSDefineDatas.toLocal().handle_;
    }
    else
    {
        return m_pJSDefineDatas.toLocal().handle_;
    }
}*/

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
}
} // namespace laya

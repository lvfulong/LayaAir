#include "GLESShaderData.h"
#include "JCConch.h"
#include "JCConchRender.h"
#include <cassert>
#include <utils/Log.h>

namespace laya
{
GLESShaderData::GLESShaderData() //: ResourceBase(JCConch::s_pConchRender->m_pShaderDataManager)
{
    _defineDatas = new DefineDatas();
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

DefineDatas *GLESShaderData::getOwnerDefineData()
{
    return _defineDatas;
}

void GLESShaderData::addDefine(ShaderDefine *define)
{
    _defineDatas->add(define);
}

void GLESShaderData::addDefines(DefineDatas *defines)
{
    _defineDatas->addDefineDatas(defines);
}

void GLESShaderData::removeDefine(ShaderDefine *define)
{
    _defineDatas->remove(define);
}

void GLESShaderData::hasDefine(ShaderDefine *define)
{
    _defineDatas->has(define);
}

void GLESShaderData::clearDefine()
{
    _defineDatas->clear();
}

void GLESShaderData::setBool(int32_t index, bool value)
{
    m_data[index] = value;
}

bool GLESShaderData::getBool(int32_t index)
{
    return std::any_cast<bool>(m_data[index]);
}

void GLESShaderData::setInt(int32_t index, int32_t value)
{
    m_data[index] = value;
}

int32_t GLESShaderData::getInt(int32_t index)
{
    return std::any_cast<int32_t>(m_data[index]);
}

void GLESShaderData::setNumber(int32_t index, float value)
{
    m_data[index] = value;
}

float GLESShaderData::getNumber(int32_t index)
{
    return std::any_cast<float>(m_data[index]);
}

void GLESShaderData::setVector2(int32_t index, const Vector2 value)
{
    m_data[index] = value;
}

Vector2 GLESShaderData::getVector2(int32_t index)
{
    return std::any_cast<Vector2>(m_data[index]);
}

void GLESShaderData::setVector(int32_t index, const Vector4 value)
{
    m_data[index] = value;
}

Vector4 GLESShaderData::getVector(int32_t index)
{
    return std::any_cast<Vector4>(m_data[index]);
}

void GLESShaderData::setVector3(int32_t index, const Vector3 value)
{
    m_data[index] = value;
}

Vector3 GLESShaderData::getVector3(int32_t index)
{
    return std::any_cast<Vector3>(m_data[index]);
}

void GLESShaderData::setColor(int32_t index, const Color value)
{
    if (&value == nullptr)
    {
        return;
    }
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

Color GLESShaderData::getColor(int32_t index)
{
    return m_gammaColorMap[index];
}

void GLESShaderData::setMatrix3x3(int32_t index, const Matrix3x3 value)
{
    m_data[index] = value;
}

Matrix3x3 GLESShaderData::getMatrix3x3(int32_t index)
{
    return std::any_cast<Matrix3x3>(m_data[index]);
}

void GLESShaderData::setMatrix4x4(int32_t index, const Matrix4x4 value)
{
    m_data[index] = value;
}

Matrix4x4 GLESShaderData::getMatrix4x4(int32_t index)
{
    return std::any_cast<Matrix4x4>(m_data[index]);
}

void GLESShaderData::setBuffer(int32_t index, uint8_t *data, uint32_t lengthInBytes)
{
    BufferDataInfo info;
    info.m_data = data;
    info.m_lengthInBytes = lengthInBytes;
    m_data[index] = info;
}

BufferDataInfo GLESShaderData::getBuffer(int32_t index)
{
    return std::any_cast<BufferDataInfo>(m_data[index]);
}

void GLESShaderData::setInternalTexture(int32_t index, GLESInternalTex *value)
{
    if (value != nullptr)
    {
        bool data = GLESEngine::_texGammaDefine.find(index) != GLESEngine::_texGammaDefine.end();
        if (data && value->gammaCorrection > 1)
        {
            addDefine(&GLESEngine::_texGammaDefine[index]);
        }
        else
        {
            if (data)
                removeDefine(&GLESEngine::_texGammaDefine[index]);
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
//------------------------------------------------------------------------------

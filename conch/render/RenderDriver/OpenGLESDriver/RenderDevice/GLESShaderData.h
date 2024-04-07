#ifndef __GLESShaderData_H__
#define __GLESShaderData_H__
#include <any>
#include <binder/JSInterface.h>
#include <core/math/Color.h>
#include <core/math/Matrix3x3.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/RTDefineDatas.h>
#include <stdio.h>
#include <unordered_map>

namespace laya
{
class BufferDataInfo
{
  public:
    uint8_t *m_data = nullptr;
    uint32_t m_lengthInBytes = 0;
};

enum class ShaderDataType
{
    None,
    Int,
    Bool,
    Float,
    Vector2,
    Vector3,
    Vector4,
    Color,
    Matrix4x4,
    Texture2D,
    TextureCube,
    Buffer,
    Matrix3x3,
};

class RTShaderDefine;
class UniformBufferObject;
class GLESInternalTex;
class GLESShaderData //: public ResourceBase<ShaderData>
{
  public:
  public:
  public:
    GLESShaderData(RTDefineDatas* defineDatas);

    ~GLESShaderData();

    template <typename T> T *getData(uint32_t key)
    {
        std::unordered_map<uint32_t, std::any>::iterator it = m_data.find(key);
        if (it != m_data.end())
        {
            // assert(it->second.type == std::typeid(T));
            return &std::any_cast<T&>(it->second);
        }
        return nullptr;
    }

    void destroy();

    //RTDefineDatas *getOwnerDefineData();
    //JsValue getOwnerDefineDataJS();
    void addDefine(RTShaderDefine define);
    void addDefines(RTDefineDatas *defines);
    void removeDefine(RTShaderDefine define);
    bool hasDefine(RTShaderDefine define);
    void clearDefine();

    void setBool(int32_t index, bool value);
    bool getBool(int32_t index);
    void setInt(int32_t index, int32_t value);
    int32_t getInt(int32_t index);
    void setNumber(int32_t index, float value);
    float getNumber(int32_t index);
    void setVector2(int32_t index, const Vector2 &value);
    const Vector2 &getVector2(int32_t index);
    void setVector(int32_t index, const Vector4 &value);
    const Vector4 &getVector(int32_t index);
    void setVector3(int32_t index, const Vector3 &value);
    const Vector3 &getVector3(int32_t index);
    void setColor(int32_t index, const Color &value);
    Color* getColor(int32_t index);
    void setMatrix4x4(int32_t index, const Matrix4x4 &value);
    const Matrix4x4 &getMatrix4x4(int32_t index);
    void setMatrix3x3(int32_t index, const Matrix3x3 &value);
    const Matrix3x3 &getMatrix3x3(int32_t index);
    void setBuffer(int32_t index, uint8_t *data, uint32_t lengthInBytes);
    void setBuffer(int32_t index, BufferDataInfo& data);
    const BufferDataInfo &getBuffer(int32_t index);
    void setInternalTexture(int32_t index, GLESInternalTex *value);
    GLESInternalTex *getInternalTexture(int32_t index);

    void cloneTo(GLESShaderData *destObject);

  private:
    bool isDestroy{false};
    std::unordered_map<uint32_t, Color> m_gammaColorMap;

  public:
    std::unordered_map<uint32_t, std::any> m_data;
    RTDefineDatas *_defineDatas = nullptr;
    //Persistent m_pJSDefineDatas;
};
} // namespace laya
#endif //__GLESShaderData_H__

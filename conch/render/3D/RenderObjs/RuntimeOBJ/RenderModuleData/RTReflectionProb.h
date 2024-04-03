#ifndef __RTReflectionProb_H__
#define __RTReflectionProb_H__

#include "render/3D/temp/Transform3D.h"
#include <core/math/Matrix4x4.h>
#include <core/math/Types.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include "Bindings/LayaAir/3D/JSBounds.h"
namespace laya
{
enum class AmbientMode
{
    /** �̶���ɫ��*/
    SolidColor,
    /** ��г����, ͨ����պ����ɵ���г���ݡ� */
    SphericalHarmonics
};

class RTReflectionProb
{
public:
    static uint32_t _idCounter;
  public:
      RTReflectionProb() { _id = RTReflectionProb::_idCounter++; };
      ~RTReflectionProb() {};
      bool boxProjection;
    uint32_t _id;
    JSBounds *bound = nullptr;
    AmbientMode ambientMode;

    float *ambientSH = nullptr;
    uint32_t ambientSHByteLength;

    Real ambientIntensity;

    Real reflectionIntensity;

    GLESInternalTex *reflectionTexture = nullptr;

    GLESInternalTex *iblTex = nullptr;
    uint32_t updateMark = 0;
    bool iblTexRGBD;
    Vector3 probePosition;
    Color ambientColor;

    void setReflectionTexture(GLESInternalTex *value)
    {
        reflectionTexture = value;
    }
    void setIblTex(GLESInternalTex *value)
    {
        iblTex = value;
    }
    void setProbePosition(Vector3 value)
    {
        value.cloneTo(this->probePosition);
    }
    void setAmbientColor(Color value)
    {
        value.cloneTo(this->ambientColor);
    }
    void setAmbientSH(float *value)
    {
        ambientSH = value;
    }
    void setBounds(JSBounds *value)
    {
        this->bound = value;
    }
    void destroy()
    {
        // todo
    }

    void applyRenderData(GLESShaderData* shaderData);
};
} // namespace laya
#endif
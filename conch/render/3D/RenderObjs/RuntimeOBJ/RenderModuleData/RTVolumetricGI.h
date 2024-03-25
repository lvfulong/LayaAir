#ifndef __RTVolumetricGI_H__
#define __RTVolumetricGI_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include <core/math/Types.h>

namespace laya
{
class JSBounds;
class RTVolumetricGI
{
  public:
      RTVolumetricGI() {};
      ~RTVolumetricGI() {};
      GLESInternalTex* irradiance = nullptr;
      GLESInternalTex* distance = nullptr;
      JSBounds* bound = nullptr;
      Real intensity;
      uint32_t  updateMark;
      Vector3 probeCounts;
      Vector3 probeStep;
      Vector4 params;

      void setIrradiance(GLESInternalTex* value) { irradiance = value; }
      void setDistance(GLESInternalTex* value) { distance = value; }
      void setBounds(JSBounds* value) { bound = value; }
      void setParams(Vector4 value)
      {
          params = value;
      }
      void setProbeCounts(Vector3 value)
      {
          probeCounts = value;
      }

      void setProbeStep(Vector3 value)
      {
          probeStep = value;
      }

      void applyRenderData(GLESShaderData* shaderData);
};
} // namespace laya
#endif
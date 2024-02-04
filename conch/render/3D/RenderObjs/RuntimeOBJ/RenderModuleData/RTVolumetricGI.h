#ifndef __RTVolumetricGI_H__
#define __RTVolumetricGI_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/ShadowMode.h>
#include <core/math/Types.h>
#include "LayaAir/3D/JSBounds.h"
namespace laya
{
class Transform3D;
class RTVolumetricGI
{
  public:
      GLESInternalTex* irradiance;
      GLESInternalTex* distance;
      JSBounds* bound;
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
};
} // namespace laya
#endif
#ifndef __RTLightmapData_H__
#define __RTLightmapData_H__

#include <core/math/Matrix4x4.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>
#include <render/3D/ShadowMode.h>
#include <core/math/Types.h>

namespace laya
{
class Transform3D;
class RTLightmapData
{
  public:
	  GLESInternalTex* lightmapColor;// InternalTexture;
	  GLESInternalTex* lightmapDirection;// InternalTexture;

	  void setLightmapColor(GLESInternalTex* value) { lightmapColor = value; }
	  void setLightmapDirection(GLESInternalTex* value) { lightmapDirection = value; }
	  void destroy()
	  {
		  lightmapColor = nullptr;
		  lightmapDirection = nullptr;
	  }
};
} // namespace laya
#endif
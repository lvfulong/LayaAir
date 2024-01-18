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
	  WebGLInternalTex* lightmapColor;// InternalTexture;
	  WebGLInternalTex* lightmapDirection;// InternalTexture;

	  void setLightmapColor(WebGLInternalTex* value) { lightmapColor = value; }
	  void setLightmapDirection(WebGLInternalTex* value) { lightmapDirection = value; }
	  void destroy()
	  {
		  lightmapColor = nullptr;
		  lightmapDirection = nullptr;
	  }
};
} // namespace laya
#endif
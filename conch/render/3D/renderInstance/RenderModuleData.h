#ifndef __RenderModuleData_H_
#define __RenderModuleData_H_
#include "core/math/Vector3.h"
#include "core/math/Vector4.h"
#include "core/math/Bounds.h"
#include "core/math/Color.h"
namespace laya{
    //Base class for modular parameters
    class RenderModuleData {
    public:
        RenderModuleData();
        virtual ~RenderModuleData();
        virtual void applyRenderData(uint32_t shaderData) = 0;
        //bind set Script function
        void set_updataMask(uint32_t value);
    public:
        uint32_t updataMask = 0;

    };

    class ReflectiobProbe :public RenderModuleData {
      
    public:
        enum class AmbientMode {
            /** 固定颜色。*/
            SolidColor,
            /** 球谐光照, 通过天空盒生成的球谐数据。 */
            SphericalHarmonics,
            /** 分别设置天空, 地平线, 地面的环境光颜色 */
            TripleColor
        };
        ReflectiobProbe();
        ~ReflectiobProbe();
        void applyRenderData(uint32_t shaderData) override;
        //TODO  bind set Script function
    public:
        bool boxProjection;
        Vector3 probePosition;
        Bounds* bound;
        AmbientMode ambientMode;
        Color abientColor;
        uint32_t iblTex;//textureCube id
        float ambientSH[27];
        float ambientIntensity;
        float reflectionIntensity;

        //Legency 
        uint32_t reflectionTexture;
        Vector4 reflectionHDRParams;
        Vector4 shCoefficients_0;
        Vector4 shCoefficients_1;
        Vector4 shCoefficients_2;
        Vector4 shCoefficients_3;
        Vector4 shCoefficients_4;
        Vector4 shCoefficients_5;
        Vector4 shCoefficients_6;
    };


    class VolumetricGI :public RenderModuleData {
    public:
        VolumetricGI();
        ~VolumetricGI();
        void applyRenderData(uint32_t shaderData) override;
        //TODO  bind set Script function
    public:

        //script set Data
        //texture id
        uint32_t irradiance;
        uint32_t distance;

        float normalBias;
        float viewBais;
        Vector3 probeCounts;
        Vector3 probeStep;

        Bounds* bound;
        float intensity;

    };

    class Lightmap {
    public:
        /** 光照贴图颜色。 */
        uint32_t lightmapColor;
        /** 光照贴图方向。 */
        uint32_t lightmapDirection;
        //TODO  bind set Script function
    };
}

#endif
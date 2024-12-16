#ifndef __GLESRenderCMD_H_
#define __GLESRenderCMD_H_
#include <vector>
#include <core/math/Vector4.h>
#include <core/math/Vector3.h>
#include <core/math/Quaternion.h>
#include <core/math/Matrix4x4.h>
#include <core/math/Matrix3x3.h>
#include <core/math/Color.h>
#include <any>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
namespace laya{
    class GLESRenderCMD {
   /* public:
        static void applyCommandBuffers(void* context, std::vector<std::vector<GLESRenderCMD*>>& commandbuffers);*/
    public:
        virtual ~GLESRenderCMD() {};
        virtual void apply(void* context) = 0;
    };


    class GLESSetRenderData : GLESRenderCMD {
    public:
        GLESSetRenderData();
        ~GLESSetRenderData();
        void apply(void* context) override;
        void setInt(int32_t value) {
            _value = value;
        };
        void setFloat(float value) {
            _value = value;
        };
        void setBool(bool value) {
            _value = value;
        };
        void setVector2(const Vector2& value) {
            _value = value;
        };
        void setVector3(const Vector3& value) {
            _value = value;
        };
        void setVector(const Vector4& value) {
            _value = value;
        };
        void setMatrix4x4(const Matrix4x4& value) {
            _value = value;
        };
        void setColor(const Color& value) {
            _value = value;
        }
        void setTexture2D(GLESInternalTex* value) {
            _value = value;
        }

        void setBufferValueJS(jsbind::ArrayBuffer arrayBuffer);
        void setBufferValue(uint8_t* data, uint32_t lengthInBytes);
        void setDataType(ShaderDataType type) { _type = type; };
        void setPropertyID(uint32_t value) { _propertyID = value; };
        void setDest(GLESShaderData* data) { _dest = data; };
    private:
        std::any _value;
        uint32_t _propertyID;
        ShaderDataType _type;
        GLESShaderData* _dest;
    };


    class GLESSetShaderDefine :GLESRenderCMD {
    public:
        GLESSetShaderDefine();
        ~GLESSetShaderDefine();
        void apply(void* context) override;
        void setDefine(const RTShaderDefine& define) { _define = define; };
        void setDest(GLESShaderData* dest) { _dest = dest; };
        void setAdd(bool value) { _add = value; };
    private:
        RTShaderDefine _define{};
        GLESShaderData* _dest = nullptr;
        bool _add;
    };


}
#endif

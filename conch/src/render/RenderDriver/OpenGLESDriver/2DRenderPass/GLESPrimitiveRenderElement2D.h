#ifndef __GLESPrimitiveRenderElement2D_H__
#define __GLESPrimitiveRenderElement2D_H__

#include "GLESRenderElement2D.h"

namespace laya
{
    class GLESShaderData;
    class GLESRenderContext2D;
    class GLESShaderInstance;

    /**
     * @brief OpenGL ES 2D图元渲染元素类
     * 
     * 继承自GLESRenderElement2D，专门处理2D图元渲染，
     * 包含额外的primitiveShaderData用于图元着色器数据管理
     */
    class GLESPrimitiveRenderElement2D : public GLESRenderElement2D
    {
    public:
        /**
         * @brief 构造函数
         */
        GLESPrimitiveRenderElement2D();
        
        /**
         * @brief 析构函数
         */
        virtual ~GLESPrimitiveRenderElement2D();

        /**
         * @brief 设置图元着色器数据
         * @param value 图元着色器数据指针
         */
        void setPrimitiveShaderData(GLESShaderData* value);

        /**
         * @brief 获取图元着色器数据
         * @return 图元着色器数据指针
         */
        GLESShaderData* getPrimitiveShaderData() const;

    public:
        /**
         * @brief 图元着色器数据
         */
        GLESShaderData* primitiveShaderData = nullptr;

    protected:
        /**
         * @brief 编译着色器
         * @param context 2D渲染上下文
         */
        void _compileShader(GLESRenderContext2D* context) override;

        /**
         * @brief 通过着色器实例进行渲染
         * @param shader 着色器实例
         * @param context 2D渲染上下文
         */
        void _renderByShaderInstance(GLESShaderInstance* shader, GLESRenderContext2D* context) override;

    private:
        /**
         * @brief 获取用于渲染状态的着色器数据
         * @return 着色器数据指针
         */
        GLESShaderData* _getRenderStateShaderData();
    };

} // namespace laya

#endif // __GLESPrimitiveRenderElement2D_H__ 
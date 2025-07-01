#include "GLESPrimitiveRenderElement2D.h"
#include "GLESRenderContext2D.h"
#include "render/LayaGL.h"
#include "render/Property.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderInstance.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h"
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h"
#include "render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTShaderPass.h"

namespace laya
{

   GLESPrimitiveRenderElement2D::GLESPrimitiveRenderElement2D()
       : GLESRenderElement2D()
   {
      primitiveShaderData = nullptr;
   }

   GLESPrimitiveRenderElement2D::~GLESPrimitiveRenderElement2D()
   {
      primitiveShaderData = nullptr;
   }

   void GLESPrimitiveRenderElement2D::setPrimitiveShaderData(GLESShaderData *value)
   {
      primitiveShaderData = value;
   }

   GLESShaderData *GLESPrimitiveRenderElement2D::getPrimitiveShaderData() const
   {
      return primitiveShaderData;
   }

   void GLESPrimitiveRenderElement2D::_compileShader(GLESRenderContext2D *context)
   {
      if (!subShader)
         return;

      std::vector<RTShaderPass *> passes = subShader->shaderpasses;
      _shaderInstances.clear();

      for (uint32_t j = 0, m = passes.size(); j < m; j++)
      {
         RTShaderPass *pass = passes[j];


         // 检查管线模式是否匹配
         if (pass->pipelineMode != context->pipelineMode)
            continue;

         RTDefineDatas *comDef = GLESRenderElement2D::_compileDefines;
         // 设置全局着色器数据
         GLESShaderData *global = getGlobalShaderData();
         if (global)
         {
            global->_defineDatas->cloneTo(comDef);
         }
         else if (context->globalConfigShaderData)
         {
            context->globalConfigShaderData->cloneTo(comDef);
         }

         // 添加pass数据定义
         if (context->passData)
         {
            comDef->addDefineDatas(context->passData->_defineDatas);
         }

         // 处理伽马校正
         bool returnGamma = !(context->destRT) || (context->destRT->m_textures.front()->gammaCorrection != 1);
         if (returnGamma)
         {
            comDef->add(ShaderDefines2D::GAMMASPACE);
         }
         else
         {
            comDef->remove(ShaderDefines2D::GAMMASPACE);
         }

         // 处理Y轴翻转
         if (context->invertY)
         {
            comDef->add(ShaderDefines2D::INVERTY);
         }
         else
         {
            comDef->remove(ShaderDefines2D::INVERTY);
         }

         // 添加2D着色器数据定义
         if (value2DShaderData)
         {
            comDef->addDefineDatas(value2DShaderData->_defineDatas);
            pass->nodeCommonMap = _commonUniformMap;
         }

         // 添加材质着色器数据定义
         if (materialShaderData)
         {
            comDef->addDefineDatas(materialShaderData->_defineDatas);
         }

         // 添加图元着色器数据定义
         if (primitiveShaderData)
         {
            comDef->addDefineDatas(primitiveShaderData->_defineDatas);
         }

         // 获取或创建着色器实例
         pass->is2D = true;
         RTShaderPass::CacheShaderItem *item = pass->getCacheShader(comDef);
         GLESShaderInstance *shader;
         if (item)
            shader = item->_glesShaderInstance;
         else
            shader = pass->callCreateShaderInstanceFunction();

         assert(shader != nullptr);
         _shaderInstances.add(shader);
      }
   }

   void GLESPrimitiveRenderElement2D::_renderByShaderInstance(GLESShaderInstance *shader, GLESRenderContext2D *context)
   {
      if (!shader->complete())
         return;

      // 绑定着色器
      shader->bind();
      _uploadGlobalAndPass(shader, context);
      // 上传2D着色器数据
      if (value2DShaderData)
      {
         shader->uploadUniforms(&(shader->m_sprite2DUniformParamsMap), value2DShaderData, true);
      }

      // 上传材质着色器数据
      if (materialShaderData)
      {
         shader->uploadUniforms(&(shader->m_materialUniformParamsMap), materialShaderData, true);
      }

      // 上传图元着色器数据
      if (primitiveShaderData)
      {
         CommandEncoder primitiveUniformParamsMap = shader->_additionUniformParamsMaps.find("Sprite2DGraphics")->second;
         shader->uploadUniforms(&primitiveUniformParamsMap, primitiveShaderData, true);
      }

      // 获取用于渲染状态的着色器数据
      GLESShaderData *renderStateShaderData = _getRenderStateShaderData();

      // 上传渲染状态
      if (renderStateShaderData)
      {
         shader->uploadRenderStateBlendDepth(renderStateShaderData);
         shader->uploadRenderStateFrontFace(renderStateShaderData, false, context->invertY);
      }

      // 执行几何体绘制
      LayaGL::m_pWebglEngine->getDrawContext()->drawGeometryElement(geometry);
   }

   GLESShaderData *GLESPrimitiveRenderElement2D::_getRenderStateShaderData()
   {
      // 按优先级选择用于渲染状态的着色器数据
      if (!renderStateIsBySprite)
      {
         if (materialShaderData)
         {
            return materialShaderData;
         }
         else if (primitiveShaderData)
         {
            return primitiveShaderData;
         }
      }

      return value2DShaderData;
   }

} // namespace laya
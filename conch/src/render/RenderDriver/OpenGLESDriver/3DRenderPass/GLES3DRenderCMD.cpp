#include "GLES3DRenderCMD.h"
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include <render/RenderDriver/RenderModuleData/RuntimeModuleData/3D/RTSubShader.h>
#include<render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderContext3D.h>
#include<render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include<render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalRT.h>
#include<render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h>
#include<render/3D/RenderObjs/RuntimeOBJ/Render3DNode/RTBaseRenderNode.h>
#include <render/Property.h>
namespace laya {
	void GLES3DRenderCMD::applyCommandBuffers(GLESRenderContext3D* context, std::vector<std::vector<GLESRenderCMD*>>& commandbuffers) {
		if (commandbuffers.size() > 0) {
			for (auto i : commandbuffers) {
				context->runCMDList(i);
			}
		}
	}


	//GLESDrawNodeCMDData
	GLESDrawNodeCMDData::GLESDrawNodeCMDData()
	{

	}
	GLESDrawNodeCMDData::~GLESDrawNodeCMDData()
	{
		_node = nullptr;
		_shaderData = nullptr;
		_subShader = nullptr;
	}
	void GLESDrawNodeCMDData::apply(void* context)
	{
		GLESRenderContext3D* ctx = static_cast<GLESRenderContext3D*>(context);
		//void apply(void* context) override;
		if (this->_shaderData && this->_subShader) {
			_node->_renderUpdatePre(ctx);
			if (this->_subMeshIndex == -1) {

				for (auto element : _node->renderelements) {
					RTSubShader* oriElement = element->subshader;
					GLESShaderData* oriMatShaderData = element->materialShaderData;
					element->subshader = _subShader;
					element->materialShaderData = _shaderData;
					ctx->drawRenderElementOne(element);
					element->subshader = oriElement;
					element->materialShaderData = oriMatShaderData;
				}
			}
			else {
				auto element = _node->renderelements[this->_subMeshIndex];
				RTSubShader* oriSubShader = element->subshader;
				GLESShaderData* oriMatShaderData = element->materialShaderData;
				element->subshader = this->_subShader;
				element->materialShaderData = this->_shaderData;
				ctx->drawRenderElementOne(element);
				element->subshader = oriSubShader;
				element->materialShaderData = oriMatShaderData;
			}
		}

	}

	void GLESDrawNodeCMDData::setBaseRenderNode(RTBaseRenderNode* node)
	{
		_node = node;
	}
	void GLESDrawNodeCMDData::setShaderData(GLESShaderData* shaderData)
	{
		_shaderData = shaderData;
	}
	void GLESDrawNodeCMDData::setSubShader(RTSubShader* subShader)
	{
		_subShader = subShader;
	}
	void GLESDrawNodeCMDData::setSubMeshIndex(int32_t index)
	{
		_subMeshIndex = index;
	}
	//GLESBlitQuadCMDData

	GLESBlitQuadCMDData::GLESBlitQuadCMDData()
	{
	}

	GLESBlitQuadCMDData::~GLESBlitQuadCMDData()
	{
		_dest = nullptr;
		_source = nullptr;
		_element = nullptr;
	}

	void GLESBlitQuadCMDData::apply(void* context)
	{
		GLESRenderContext3D* ctx = static_cast<GLESRenderContext3D*>(context);
		_element->materialShaderData->setInternalTexture(CommandProperty::SCREENTEXTURE_ID, _source);
		_element->materialShaderData->setVector(CommandProperty::SCREENTEXTUREOFFSETSCALE_ID, _offsetScale);
		_element->materialShaderData->setVector(CommandProperty::MAINTEXTURE_TEXELSIZE_ID, _sourceTexelSize);
		ctx->setViewport(_viewport);
		ctx->setScissor(_scissor);
		ctx->setRenderTarget(_dest, static_cast<uint32_t>(RenderClearFlag::Nothing));
		ctx->drawRenderElementOne(_element);
	}

	void GLESBlitQuadCMDData::setSource(GLESInternalTex* source)
	{
		_source = source;

		_sourceTexelSize.setValue(1.0 / _source->m_width, 1.0 / _source->m_height, _source->m_width, _source->m_height);
	}



	//GLESDrawElementCMDData
	GLESDrawElementCMDData::GLESDrawElementCMDData()
	{
	}

	GLESDrawElementCMDData::~GLESDrawElementCMDData()
	{
		_elements.clear();
	}

	void GLESDrawElementCMDData::apply(void* context) {
		GLESRenderContext3D* ctx = static_cast<GLESRenderContext3D*>(context);
		for (auto element : _elements) {
			ctx->drawRenderElementOne(element);
		}
	}

	void GLESDrawElementCMDData::clearElement()
	{
		_elements.clear();
	}

	void GLESDrawElementCMDData::addOneElement(GLESRenderElement3D* element)
	{
		_elements.push_back(element);
	}

	//GLESSetViewportCMD
	GLESSetViewportCMD::GLESSetViewportCMD() {
	}

	GLESSetViewportCMD::~GLESSetViewportCMD() {
	}

	void GLESSetViewportCMD::apply(void* context) {
		GLESRenderContext3D* ctx = static_cast<GLESRenderContext3D*>(context);
		ctx->setViewport(_viewport);
		ctx->setScissor(_scissor);
	}

	//GLESSetRenderTargetCMD
	GLESSetRenderTargetCMD::GLESSetRenderTargetCMD() {
	}

	GLESSetRenderTargetCMD::~GLESSetRenderTargetCMD() {

	}

	void GLESSetRenderTargetCMD::apply(void* context) {
		GLESRenderContext3D* ctx = static_cast<GLESRenderContext3D*>(context);
		ctx->setRenderTarget(_rt);
		ctx->setClearData(_flag, _clearColor, _clearDepth, _clearStencilValue);
		if (_rt != nullptr) {
			GLESInternalTex* tex = static_cast<GLESInternalTex*>(_rt->m_textures[0]);
			Viewport vp(0, 0, tex->m_width, tex->m_height);
			Vector4 scissor(0, 0, tex->m_width, tex->m_height);
			ctx->setViewport(vp);
			ctx->setScissor(scissor);
		}
	}
}

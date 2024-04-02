#include "GLESRenderCMD.h"
#include <render/RenderDriver/OpenGLESDriver/3DRenderPass/GLESRenderElement3D.h>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESInternalTex.h"
#include "Render3DNode/RTBaseRenderNode.h"
#include <render/Property.h>
namespace laya{
	void GLESRenderCMD::applyCommandBuffers(GLESRenderContext3D* context,std::vector<std::vector<GLESRenderCMD*>>& commandbuffers) {
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
	void GLESDrawNodeCMDData::apply(GLESRenderContext3D* context)
	{
		_node->_renderUpdatePre(context);
		for (auto element : _node->renderelements) {
				RTSubShader* oriElement = element->subshader;
				GLESShaderData* oriMatShaderData = element->materialShaderData;
				element->subshader = _subShader;
				element->materialShaderData = _shaderData;
				context->drawRenderElementOne(element);
				element->subshader = oriElement;
				element->materialShaderData = oriMatShaderData;
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

	void GLESBlitQuadCMDData::apply(GLESRenderContext3D* context)
	{
		_element->materialShaderData->setInternalTexture(CommandProperty::SCREENTEXTURE_ID, _source);
		_element->materialShaderData->setVector(CommandProperty::SCREENTEXTUREOFFSETSCALE_ID, _offsetScale);
		_element->materialShaderData->setVector(CommandProperty::MAINTEXTURE_TEXELSIZE_ID, _sourceTexelSize);
		context->setViewport(_viewport);
		context->setScissor(_scissor);
		context->setRenderTarget(_dest);
		context->drawRenderElementOne(_element);
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

	void GLESDrawElementCMDData::apply(GLESRenderContext3D* context) {
		for (auto element : _elements) {
			context->drawRenderElementOne(element);
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

	void GLESSetViewportCMD::apply(GLESRenderContext3D* context) {
		context->setViewport(_viewport);
		context->setScissor(_scissor);
	}

	//GLESSetRenderTargetCMD
	GLESSetRenderTargetCMD::GLESSetRenderTargetCMD() {
	}

	GLESSetRenderTargetCMD::~GLESSetRenderTargetCMD() {

	}

	void GLESSetRenderTargetCMD::apply(GLESRenderContext3D* context) {
		context->setRenderTarget(_rt);
		context->setClearData(_flag,_clearColor, _clearDepth, _clearStencilValue);
	}

	GLESSetRenderData::GLESSetRenderData() {
	}

	GLESSetRenderData::~GLESSetRenderData(){
	}
	void GLESSetRenderData::setBufferValueJS(JSValueAsParam value) 
	{
		char* pArrayBufferPtr = NULL;
		int nABLen = 0;
		bool bIsArrayBuffer = extractJSAB(value, pArrayBufferPtr, nABLen);
		if (bIsArrayBuffer)
		{
			setBufferValue((uint8_t*)pArrayBufferPtr, nABLen);
		}
	}
	void GLESSetRenderData::setBufferValue(uint8_t* data, uint32_t lengthInBytes) {
		BufferDataInfo info;
		info.m_data = data;
		info.m_lengthInBytes = lengthInBytes;
		_value = info;
	}
	void GLESSetRenderData::apply(GLESRenderContext3D* context) {
		switch (_type) {
		case ShaderDataType::Int:
			_dest->setInt(_propertyID, std::any_cast<int32_t>(_value));
			break;
		case ShaderDataType::Float:
			_dest->setNumber(_propertyID  , std::any_cast<float>(_value)  );
			break;
		case ShaderDataType::Bool:
			_dest->setBool(_propertyID  ,  std::any_cast<bool>(_value));
			break;
		case ShaderDataType::Matrix4x4:
			_dest->setMatrix4x4(_propertyID  ,  std::any_cast<Matrix4x4>(_value) );
			break;
		case ShaderDataType::Color:
			_dest->setColor(_propertyID  ,  std::any_cast<Color>(_value));
			break;
		case ShaderDataType::Texture2D:
			_dest->setInternalTexture(_propertyID  ,  std::any_cast<GLESInternalTex*>(_value));
			break;
		case ShaderDataType::Vector4:
			_dest->setVector(_propertyID  ,  std::any_cast<Vector4>(_value));
			break;
		case ShaderDataType::Vector2:
			_dest->setVector2(_propertyID  ,  std::any_cast<Vector2>(_value));
			break;
		case ShaderDataType::Vector3:
			_dest->setVector3(_propertyID  ,  std::any_cast<Vector3>(_value));
			break;
		case ShaderDataType::Buffer:
			_dest->setBuffer(_propertyID  , std::any_cast<BufferDataInfo&>(_value));
			break;
		default:
			//TODO  shaderDefine
			break;
		};
	}


	//GLESSetShaderDefine
	GLESSetShaderDefine::GLESSetShaderDefine() {
	}

	GLESSetShaderDefine::~GLESSetShaderDefine() {
		_dest = nullptr;
	}

	void GLESSetShaderDefine::apply(GLESRenderContext3D* context)
	{
		if (_add) {
			_dest->addDefine(_define);
		}
		else {
			_dest->removeDefine(_define);
		}
	}
}
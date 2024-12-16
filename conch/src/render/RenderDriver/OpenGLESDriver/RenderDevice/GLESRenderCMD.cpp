#include "GLESRenderCMD.h"
#include <render/Property.h>
#include <any>
#include <render/RenderDriver/OpenGLESDriver/RenderDevice/GLESShaderData.h>
namespace laya {
	GLESSetRenderData::GLESSetRenderData() {
	}

	GLESSetRenderData::~GLESSetRenderData() {
	}
	void GLESSetRenderData::setBufferValueJS(jsbind::ArrayBuffer arrayBuffer)
	{
		DEBUG_CHECK(arrayBuffer.isValid());
		setBufferValue(arrayBuffer.getData(), arrayBuffer.getByteLength());
	}
	void GLESSetRenderData::setBufferValue(uint8_t* data, uint32_t lengthInBytes) {
		BufferDataInfo info;
		info.m_data = data;
		info.m_lengthInBytes = lengthInBytes;
		_value = info;
	}
	void GLESSetRenderData::apply(void* context) {
		switch (_type) {
		case ShaderDataType::Int:
			_dest->setInt(_propertyID, std::any_cast<int32_t>(_value));
			break;
		case ShaderDataType::Float:
			_dest->setNumber(_propertyID, std::any_cast<float>(_value));
			break;
		case ShaderDataType::Bool:
			_dest->setBool(_propertyID, std::any_cast<bool>(_value));
			break;
		case ShaderDataType::Matrix4x4:
			_dest->setMatrix4x4(_propertyID, std::any_cast<Matrix4x4>(_value));
			break;
		case ShaderDataType::Color:
			_dest->setColor(_propertyID, std::any_cast<Color>(_value));
			break;
		case ShaderDataType::Texture2D:
			_dest->setInternalTexture(_propertyID, std::any_cast<GLESInternalTex*>(_value));
			break;
		case ShaderDataType::Vector4:
			_dest->setVector(_propertyID, std::any_cast<Vector4>(_value));
			break;
		case ShaderDataType::Vector2:
			_dest->setVector2(_propertyID, std::any_cast<Vector2>(_value));
			break;
		case ShaderDataType::Vector3:
			_dest->setVector3(_propertyID, std::any_cast<Vector3>(_value));
			break;
		case ShaderDataType::Buffer:
			_dest->setBuffer(_propertyID, std::any_cast<BufferDataInfo&>(_value));
			break;
		default:
			//TODO  shaderDefine
			break;
		};
	}


	//GLESSetShaderDefine
	GLESSetShaderDefine::GLESSetShaderDefine() {
		_add = false;
	}

	GLESSetShaderDefine::~GLESSetShaderDefine() {
		_dest = nullptr;
	}

	void GLESSetShaderDefine::apply(void* context)
	{
		if (_add) {
			_dest->addDefine(_define);
		}
		else {
			_dest->removeDefine(_define);
		}
	}
}

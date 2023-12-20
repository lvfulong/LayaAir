#include "GLShaderInstance.h"
#include <unordered_map>
#include "GLObject.h"
#include <assert.h>
#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif LINUX
#include <glad/egl.h>
#include <glad/gles2.h>
#else
#include <GLES3/gl3.h>
#endif
#include <Utils/Log.h>
#include "JCConch.h"
#include "WebGLInternalTex.h"
#include "JCSystemConfig.h"

namespace laya
{
	GLShaderInstance::GLShaderInstance(WebGLEngine* engine, const char* vs, const char* ps, GLAttributeMap* pAttributeMap): GLObject(engine)
	{
		m_vs = vs;
		m_ps = ps;
		m_pAttributeMap = pAttributeMap;
		m_uniformMap.clear();
		_create();
	}
	GLShaderInstance::~GLShaderInstance()
	{
		destroy();
	}
	void GLShaderInstance::_create()
	{
		m_program = glCreateProgram();
		m_vshader = _createShader(m_vs, GL_VERTEX_SHADER);
		m_pshader = _createShader(m_ps, GL_FRAGMENT_SHADER);
		glAttachShader(m_program, m_vshader);
		glAttachShader(m_program, m_pshader);

		std::map<std::string, int> attributes = m_pAttributeMap->getAttributeMap();
		for (std::map<std::string, int>::iterator it = attributes.begin(); it != attributes.end(); it++)
		{
			glBindAttribLocation(m_program, it->second, it->first.c_str());
		}

		glLinkProgram(m_program);

		GLint linkStatus = 0;
		glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) 
		{
			GLint bufLength = 0;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = new char[bufLength + 1];
				buf[bufLength] = '\0';

				glGetProgramInfoLog(m_program, bufLength, NULL, buf);
                std::string info = buf;
				LOGE("GLShaderInstance::compile Could not link program:\n%s\n", info.c_str());
				//assert("'Could not compile WebGL program. \n\n");
				delete[] buf;
			}
		}
		//Uniform
		//Unifrom Objcet
		int nUniformNum = 0;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &nUniformNum);

		useProgram();
		m_curActTexIndex = 0;
		char sVarName[64];
		for (int i = 0; i < nUniformNum; i++) 
		{
			//var uniformData : WebGLActiveInfo = gl.getActiveUniform(this._program, i);

			GLenum type;
			int length, count;
			glGetActiveUniform(m_program, i, sizeof(sVarName), &length, &count, &type, sVarName);
			int location = glGetUniformLocation(m_program, sVarName);
			if (!location && location != 0)
				continue;
			ShaderVariable* one = new ShaderVariable();
			one->location = location;

			int nVarNameLenght = strlen(sVarName);
			if (nVarNameLenght > 4 && strcmp(sVarName + (nVarNameLenght - 3), "[0]") == 0)
			{
				sVarName[nVarNameLenght - 3] = 0;
		
				one->name = sVarName;
				one->isArray = true;
			}
			else 
			{
				one->name = sVarName;
				one->isArray = false;
			}
			one->type = type;
			one->count = count;
			_addShaderUnifiormFun(one);
			m_uniformMap.push_back(one);
			one->dataOffset = m_engine->propertyNameToID(sVarName);
		}
		if (m_engine->isWebGL2())
		{
			m_uniformObjectMap.clear();

			GLint nUniformBlock = 0;
			glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_BLOCKS, &nUniformBlock);

			for (int i = 0; i < nUniformBlock; i++)
			{
				const GLsizei nBufferSize = 256;
				char uniformBlockName[nBufferSize] = { 0 };
				GLsizei nLength = 0;
				//GLint nActiveUniforms = 0;
				//glGetActiveUniformBlockiv(m_program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nActiveUniforms);
				glGetActiveUniformBlockName(m_program, i, nBufferSize, &nLength, uniformBlockName);
				ShaderVariable* one = new ShaderVariable();
				one->name = uniformBlockName;
				one->isArray = false;
				one->type = GL_UNIFORM_BUFFER;
				one->dataOffset = m_engine->propertyNameToID(uniformBlockName);
				int location = one->location = glGetUniformBlockIndex(m_program, uniformBlockName);
				/*if (!!UniformBufferObject.getBuffer(uniformBlockName, 0))//TODO
				{
					GLint nBytelength;
					glGetActiveUniformBlockiv(m_program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &nBytelength);

					let indexPoint = UniformBufferObject.getBuffer(uniformBlockName, 0);
					if (nBytelength != indexPoint.byteLength)
					{
						LOGE("The length of the same UBO is not uniform");
					}
					glUniformBlockBinding(m_program, location, indexPoint._glPointer);
				}
				else
				{ 
					GLint bytelength = 0;
					glGetActiveUniformBlockiv(m_program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &bytelength);
					//var bytelength : number = glGetActiveUniformBlockParameter(m_program, i, GL_UNIFORM_BLOCK_DATA_SIZE);
					let buffer : UniformBufferObject = UniformBufferObject.creat(uniformBlockName, BufferUsage.Dynamic, bytelength, UniformBufferObject.isCommon(uniformBlockName));
					glUniformBlockBinding(m_program, location, buffer._glPointer);
				}*/
				glUniformBlockBinding(m_program, location, m_engine->getUBOPointer(uniformBlockName));
				m_uniformObjectMap[one->name] = one;
				m_uniformMap.push_back(one);
				_addShaderUnifiormFun(one);
			}
		}
	}

	GLuint GLShaderInstance::_createShader(const std::string& str, int type)
	{
		GLuint shader = glCreateShader(type);
		const char* strShaders[1] = { str.c_str() };
		glShaderSource(shader, 1, strShaders, NULL);
		glCompileShader(shader);
		if (m_engine->m_isShaderDebugMode)
		{
			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled)
			{
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				if (infoLen) 
				{
					char* buf = new char[infoLen + 1];
					buf[infoLen] = '\0';

					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					//assert(true);
                    std::string info = buf;
					LOGI("GLShaderInstance _createShader %s", info.c_str());
					delete[] buf;
				}
			}
		}
		return shader;
	}
	void GLShaderInstance::_addShaderUnifiormFun(ShaderVariable* one)
	{
		bool isArray = one->isArray;
		switch (one->type)
		{
			case GL_BOOL:
				one->fun = std::bind(&GLShaderInstance::_uniform1i, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLint) * one->count;
				one->uploadedValue.resize(sizeof(GLint), 0);
				break;
			case GL_INT:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniform1iv, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniform1i, this, std::placeholders::_1, std::placeholders::_2);//TODO:�Ż�
				one->byteSize = sizeof(GLint) * one->count;
				one->uploadedValue.resize(sizeof(GLint), 0);
				break;
			case GL_FLOAT:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniform1fv, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniform1f, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * one->count;
				one->uploadedValue.resize(sizeof(GLfloat));
				break;
			case GL_FLOAT_VEC2:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniform_vec2v, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniform_vec2, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 2 * one->count;
				one->uploadedValue.resize(sizeof(GLfloat) * 2, 0);
				break;
			case GL_FLOAT_VEC3:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniform_vec3v, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniform_vec3, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 3 * one->count;
				one->uploadedValue.resize(sizeof(GLfloat) * 3, 0);
				break;
			case GL_FLOAT_VEC4:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniform_vec4v, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniform_vec4, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 4 * one->count;
				one->uploadedValue.resize(sizeof(GLfloat) * 4, 0);
				break;
			case GL_FLOAT_MAT2:
				one->fun = std::bind(&GLShaderInstance::_uniformMatrix2fv, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 4 * one->count;
				//one->uploadedValue.resize(one->byteSize, 0);
				break;
			case GL_FLOAT_MAT3:
				one->fun = std::bind(&GLShaderInstance::_uniformMatrix3fv, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 9 * one->count;
				//one->uploadedValue.resize(one->byteSize, 0);
				break;
			case GL_FLOAT_MAT4:
				one->fun = isArray ? std::bind(&GLShaderInstance::_uniformMatrix4fv, this, std::placeholders::_1, std::placeholders::_2) : std::bind(&GLShaderInstance::_uniformMatrix4f, this, std::placeholders::_1, std::placeholders::_2);
				one->byteSize = sizeof(GLfloat) * 16 * one->count;
				//one->uploadedValue.resize(one->byteSize, 0);
				break;
			case GL_SAMPLER_2D:
			case GL_SAMPLER_2D_SHADOW:
				glUniform1i(one->location, m_curActTexIndex);
				one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
				one->fun = std::bind(&GLShaderInstance::_uniform_sampler2D, this, std::placeholders::_1, std::placeholders::_2);
				break;
			case GL_SAMPLER_2D_ARRAY:
				glUniform1i(one->location, m_curActTexIndex);
				one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
				one->fun = std::bind(&GLShaderInstance::_uniform_sampler2DArray, this, std::placeholders::_1, std::placeholders::_2);
				break;
			case 0x8b5f://sampler3D
				glUniform1i(one->location, m_curActTexIndex);
				one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
				one->fun = std::bind(&GLShaderInstance::_uniform_sampler3D, this, std::placeholders::_1, std::placeholders::_2);
				break;
			case GL_SAMPLER_CUBE:
				glUniform1i(one->location, m_curActTexIndex);
				one->textureID = m_engine->m_glTextureIDParams[m_curActTexIndex++];
				one->fun = std::bind(&GLShaderInstance::_uniform_samplerCube, this, std::placeholders::_1, std::placeholders::_2);
				break;
			case GL_UNIFORM_BUFFER:
				one->fun = std::bind(&GLShaderInstance::_uniform_UniformBuffer, this, std::placeholders::_1, std::placeholders::_2);
				break;
			default:
				assert("compile shader err!");
				break;
			}
	}
	bool GLShaderInstance::bind()
	{
		return useProgram();
	}
	bool GLShaderInstance::useProgram()
	{
		if (m_engine->m_glUseProgram == this)
			return false;
		glUseProgram(m_program);
		m_engine->m_glUseProgram = this;
		return true;
	}
	int GLShaderInstance::_uniform1f(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Number32);
		assert(dataInfo->byteSize >= sizeof(GLfloat));
		GLfloat* pData = (GLfloat*)dataInfo->data;
		if (memcmp(one->uploadedValue.data(), pData, sizeof(GLfloat)) != 0)
		{
			glUniform1f(one->location, *(pData));
			memcpy(one->uploadedValue.data(), pData, sizeof(GLfloat));
			return 1;
		}
		return 0;
	}
	int GLShaderInstance::_uniform1fv(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Number32Array);
		GLfloat* value = (GLfloat*)dataInfo->data;
		int count = dataInfo->byteSize / sizeof(GLfloat);
		/*if (count < 4)
		{
			GLfloat* uploadedValue = (GLfloat*)one->uploadedValue.data();
			if (uploadedValue[0] != value[0] || uploadedValue[1] != value[1] || uploadedValue[2] != value[2] || uploadedValue[3] != value[3])
			{
				glUniform1fv(one->location, count, value);
				uploadedValue[0] = value[0];
				uploadedValue[1] = value[1];
				uploadedValue[2] = value[2];
				uploadedValue[3] = value[3];
				return 1;
			}
			return 0;
		}
		else*/
		{
			glUniform1fv(one->location, count, value);
			return 1;
		}
	}
	int GLShaderInstance::_uniform_vec2(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Vector2);
		assert(dataInfo->byteSize >= sizeof(GLfloat) * 2);
		GLfloat* pData = (GLfloat*)dataInfo->data;
		if (memcmp(one->uploadedValue.data(), pData, sizeof(GLfloat) * 2) != 0)
		{
			glUniform2f(one->location, pData[0], pData[1]);
			memcpy(one->uploadedValue.data(), pData, sizeof(GLfloat) * 2);
			return 1;
		}
		return 0;
	}
	int GLShaderInstance::_uniform_vec2v(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* value = (GLfloat*)dataInfo->data;
		int count = dataInfo->byteSize / (sizeof(GLfloat) * 2);
		/*if (count < 2) 
		{
			GLfloat* uploadedValue= (GLfloat*)one->uploadedValue.data();
			if (uploadedValue[0] != value[0] || uploadedValue[1] != value[1] || uploadedValue[2] != value[2] || uploadedValue[3] != value[3]) 
			{
				glUniform2fv(one->location, count, value);
				uploadedValue[0] = value[0];
				uploadedValue[1] = value[1];
				uploadedValue[2] = value[2];
				uploadedValue[3] = value[3];
				return 1;
			}
			return 0;
		}
		else*/
		{
			glUniform2fv(one->location, count, value);
			return 1;
		}
	}
	int GLShaderInstance::_uniform_vec3(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Vector3);
		assert(dataInfo->byteSize >= sizeof(GLfloat) * 3);
		GLfloat* pData = (GLfloat*)dataInfo->data;
		if (memcmp(one->uploadedValue.data(), pData, sizeof(GLfloat) * 3) != 0)
		{
			glUniform3f(one->location, pData[0], pData[1], pData[2]);
			memcpy(one->uploadedValue.data(), pData, sizeof(GLfloat) * 3);
			return 1;
		}
		return 0;
	}

	int GLShaderInstance::_uniform_vec3v(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* pData = (GLfloat*)dataInfo->data;
		glUniform3fv(one->location, dataInfo->byteSize / (sizeof(GLfloat) * 3), pData);
		return 0;
	}
	int GLShaderInstance::_uniform_vec4(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Vector4);
//		assert(dataInfo->byteSize >= sizeof(GLfloat) * 4);
		GLfloat* pData = (GLfloat*)dataInfo->data;
		if (memcmp(one->uploadedValue.data(), pData, sizeof(GLfloat) * 4) != 0)
		{
			glUniform4f(one->location, pData[0], pData[1], pData[2], pData[3]);
			memcpy(one->uploadedValue.data(), pData, sizeof(GLfloat) * 4);
			return 1;
		}
		return 0;
	}
	int GLShaderInstance::_uniform_vec4v(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* pData = (GLfloat*)dataInfo->data;
		glUniform4fv(one->location, dataInfo->byteSize / (sizeof(GLfloat) * 4), pData);
		return 1;
	}

	int GLShaderInstance::_uniformMatrix2fv(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* pData = (GLfloat*)dataInfo->data;
		glUniformMatrix2fv(one->location, false, dataInfo->byteSize / (4 * sizeof(float)) , pData);
		return 1;
	}

	int GLShaderInstance::_uniformMatrix3fv(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* pData = (GLfloat*)dataInfo->data;
		glUniformMatrix3fv(one->location, false, dataInfo->byteSize / (9 * sizeof(float)), pData);
		return 1;
	}
	int GLShaderInstance::_uniformMatrix4f(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)dataInfo->type == ShaderDataType::Matrix4x4);
		assert(dataInfo->byteSize >= 16 * sizeof(float));
		GLfloat* pData = (GLfloat*)dataInfo->data;
		glUniformMatrix4fv(one->location, 1, false, pData);
		return 1;
	}
	int GLShaderInstance::_uniformMatrix4fv(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		GLfloat* pData = (GLfloat*)dataInfo->data;
        glUniformMatrix4fv(one->location, dataInfo->byteSize / (16 * sizeof(float)), false, pData);
        return 1;
	}
	
	int GLShaderInstance::_uniform1i(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)one->type == ShaderDataType::Number32);
		assert(dataInfo->byteSize >= sizeof(GLint));
		GLint pData = *(GLfloat*)dataInfo->data;//注意转型 脚本只有float buffer
		if (memcmp(one->uploadedValue.data(), &pData, sizeof(GLint)) != 0)
		{
			glUniform1i(one->location, pData);
			memcpy(one->uploadedValue.data(), &pData, sizeof(GLint));
			return 1;
		}
		return 0;
	}
	
	int GLShaderInstance::_uniform1iv(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		//assert((ShaderDataType)one->type == ShaderDataType::Number32Array);
		//当前引擎未使用 需要用int32Array
		GLint* pData = (GLint*)dataInfo->data;
		glUniform1iv(one->location, dataInfo->byteSize / sizeof(GLint), pData);
		return 1;
	}
	/*int _uniform_ivec2(one: any, value : any)
	{
		var uploadedValue : any[] = one.uploadedValue;
		if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1])
		{
			this._gl.uniform2i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1]);
			return 1;
		}
		return 0;
	}
	int _uniform_ivec2v(one: any, value : any)
	{
		this._gl.uniform2iv(one.location, value);
		return 1;
	}

	int _uniform_vec3i(one: any, value : any)
	{
		var uploadedValue : any[] = one.uploadedValue;
		if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1] || uploadedValue[2] != = value[2]) 
		{
			this._gl.uniform3i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1], uploadedValue[2] = value[2]);
			return 1;
		}
		return 0;
	}

	int _uniform_vec3vi(one: any, value : any)
	{
		this._gl.uniform3iv(one.location, value);
		return 1;
	}

	int _uniform_vec4i(one: any, value : any)
	{
		var uploadedValue : any[] = one.uploadedValue;
		if (uploadedValue[0] != = value[0] || uploadedValue[1] != = value[1] || uploadedValue[2] != = value[2] || uploadedValue[3] != = value[3]) 
		{
			this._gl.uniform4i(one.location, uploadedValue[0] = value[0], uploadedValue[1] = value[1], uploadedValue[2] = value[2], uploadedValue[3] = value[3]);
			return 1;
		}
		return 0;
	}
	int _uniform_vec4vi(one: any, value : any)
	{
		this._gl.uniform4iv(one.location, value);
		return 1;
	}*/

	int GLShaderInstance::_uniform_sampler2D(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		assert((ShaderDataType)dataInfo->type == ShaderDataType::Texture);
		int id = *((int*)dataInfo->data);
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(id);
		if (texture != nullptr)
		{
			_bindTexture(one->textureID, GL_TEXTURE_2D, texture);
		}
		else
		{
			LOGI("_uniform_sampler2D find no texture");
		}
		
		return 0;
	}
	int GLShaderInstance::_uniform_sampler2DArray(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		int id = *((int*)dataInfo->data);
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(id);
		if (texture != nullptr)
		{
			_bindTexture(one->textureID, GL_TEXTURE_2D_ARRAY, texture);
		}
		else
		{
			LOGI("_uniform_sampler2DArray find no texture");
		}
		return 0;

	}
	int GLShaderInstance::_uniform_sampler3D(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		assert((ShaderDataType)dataInfo->type == ShaderDataType::Texture);
		int id = *((int*)dataInfo->data);
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(id);
		if (texture != nullptr)
		{
			_bindTexture(one->textureID, GL_TEXTURE_3D, texture);
		}
		return 0;
	}

	int GLShaderInstance::_uniform_samplerCube(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		assert((ShaderDataType)dataInfo->type == ShaderDataType::Texture);
		int id = *((int*)dataInfo->data);
		WebGLInternalTex* texture = JCConch::s_pConchRender->m_pWebGLInternalTexManager->getObject(id);
		if (texture != nullptr)
		{
			_bindTexture(one->textureID, GL_TEXTURE_CUBE_MAP, texture);
		}
		return 0;
	}
	int GLShaderInstance::_uniform_UniformBuffer(ShaderVariable* one, ShaderData::DataInfo* dataInfo)
	{
		assert((ShaderDataType)dataInfo->type == ShaderDataType::UBO);
		int id = *((int*)dataInfo->data);
		UniformBufferObject* ubo = JCConch::s_pConchRender->m_pUniformBufferObjectManager->getObject(id);
		if (ubo != nullptr)
		{
			ubo->_bindUniformBufferBase();
		}
		return 0;
	}
	void GLShaderInstance::_bindTexture(int textureID, GLenum target, WebGLInternalTex* texture)
	{
		if (g_kSystemConfig.m_bConchWebGL)
		{
			glActiveTexture(textureID);
			glBindTexture(target, texture == nullptr ? 0 : texture->getResource());
		}
		else
		{
			if (m_engine->m_activedTextureID != textureID)
			{
				glActiveTexture(textureID);
				m_engine->m_activedTextureID = textureID;
			}
			const int texID = m_engine->m_activedTextureID - GL_TEXTURE0;
			if (m_engine->m_activeTextures[texID] != texture)
			{
				glBindTexture(target, texture == nullptr ? 0 : texture->getResource());
				m_engine->m_activeTextures[texID] = texture;
			}
		}
	}

	void GLShaderInstance::destroy()
	{
		if (m_program != 0)
		{
			glDeleteShader(m_vshader);
			glDeleteShader(m_pshader);
			glDeleteProgram(m_program);
			m_vshader = 0;
			m_pshader = 0;
			m_program = 0;
			for (int i = 0, size = m_uniformMap.size(); i < size; i++)
			{
				ShaderVariable* one = m_uniformMap[i];
				delete one;
			}
			m_uniformMap.clear();
			//m_attributeMap = null;
			//m_uniformMap = null;
			//m_uniformObjectMap = null;
			//m_engine = null;
		}
	}
}

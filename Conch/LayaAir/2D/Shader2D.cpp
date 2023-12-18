#include "Shader2D.h"
#include <Utils/Log.h>
#include "Shader2DCompile.h"
#include <assert.h>
#include "../RenderEngine/RenderEngine/WebGLEngine/WebGLEngine.h"
#include "../RenderEngine/RenderInterface/IRender2DContext.h"

namespace laya
{
	std::unordered_map<int, Shader2DCompile*> Shader2D::_preCompileShader;
	std::unordered_map<uint64_t, Shader2D*> Shader2D::shaders;

	Shader2D* Shader2D::withCompile2D(WebGLEngine* pWebGLEngine, int shaderID, const Shader2DDefines& defines, const std::vector<std::pair<std::string, int> >& bindAttrib)
	{
		uint64_t cacheId = shaderID | defines.getValue();
		std::unordered_map<uint64_t, Shader2D*>::iterator it = Shader2D::shaders.find(cacheId);
		if (it != Shader2D::shaders.end())
			return it->second;

		Shader2DCompile* pre = Shader2D::_preCompileShader[shaderID];
		if (!pre)
		{
			//LOGE("withCompile shader err!");
			return nullptr;
		}
		Shader2D* shader = pre->createShader(pWebGLEngine, defines.toNameDic(), bindAttrib);
		Shader2D::shaders[cacheId] = shader;
		return shader;
	}
	Shader2D::Shader2D(WebGLEngine* pWebGLEngine, const std::string& vs, const std::string& ps, const std::vector<std::pair<std::string, int> >& bindAttrib)
	{
		//LOGI("Shader2D::Shader vs %s", vs.c_str());
		//LOGI("Shader2D::Shader ps %s", ps.c_str());
		this->_attribInfo = bindAttrib;
		//this->_id = ++Shader2D::_count;
		this->_vs = vs;
		this->_ps = ps;
		//this._nameMap = nameMap ? nameMap : {};
		//Shader2D::shaders[cacheId] = this;
		this->recreateResource();
		//this.lock = true;
		m_pRender2DContext = pWebGLEngine->get2DRenderContext();
	}
    //------------------------------------------------------------------------------
    void Shader2D::onDestory()
    {
        {
            std::unordered_map<int, Shader2DCompile*>::iterator it = Shader2D::_preCompileShader.begin();
            for (; it !=  Shader2D::_preCompileShader.end(); it++)
            {
                delete it->second;
            }
            Shader2D::_preCompileShader.clear();
        }
        {
            std::unordered_map<uint64_t, Shader2D*>::iterator it = Shader2D::shaders.begin();
            for (; it != Shader2D::shaders.end(); it++)
            {
                delete it->second;
            }
            Shader2D::shaders.clear();
        }
    }
	void Shader2D::preprocess2D(/*int nameID, */int mainID, const char* vs, const char* ps/*, nameMap : any*/)
	{
		//LOGI("vs %s", vs);
		//LOGI("ps %s", ps);
		int id = /*Shader2D::SHADERNAME2ID * nameID + */mainID;
		Shader2D::_preCompileShader[id] = new Shader2DCompile(vs, ps/*, nameMap*/);
	}
	void Shader2D::_compile()
	{
		GLenum nError = glGetError();
		if (nError != GL_NO_ERROR)
		{
			LOGE("GL_ERROR (0x%x)", nError);
		}
		//if (!this._vs || !this._ps || this._params)
		//	return;

		this->_reCompile = true;
		//this._params = [];

		//var result : any;
		//if (this.customCompile)
		//	result = ILaya.ShaderCompile.preGetParams(this._vs, this._ps);
		
		this->_vshader = Shader2D::_createShader(this->_vs, GL_VERTEX_SHADER);
		this->_pshader = Shader2D::_createShader(this->_ps, GL_FRAGMENT_SHADER);
		this->_program = glCreateProgram();
		glAttachShader(this->_program, this->_vshader);
		glAttachShader(this->_program, this->_pshader);

		//var one : any, i : number, j : number, n : number, location : any;

		//属性用指定location的方法，这样更灵活，更方便与vao结合。
		//注意注意注意 这个必须放到link前面
		int attribDescNum = this->_attribInfo.size();
		for (int i = 0; i < attribDescNum; i++) 
		{
			glBindAttribLocation(this->_program, this->_attribInfo[i].second, this->_attribInfo[i].first.c_str());
		}

		glLinkProgram(this->_program);
		//if (!this.customCompile) 
		{
			GLint linkStatus = 0;
			glGetProgramiv(this->_program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE) {
				GLint bufLength = 0;
				glGetProgramiv(this->_program, GL_INFO_LOG_LENGTH, &bufLength);
				if (bufLength) {
					char* buf = new char[bufLength];
					if (buf) {
						glGetProgramInfoLog(this->_program, bufLength, NULL, buf);
						LOGE("Shader2D::compile Could not link program:\n%s\n", buf);
						delete[] buf;
					}
				}
				//glDeleteProgram(this->_program);
				//this->_program = 0;
				//return;
			}
		}

		 nError = glGetError();
		if (nError != GL_NO_ERROR)
		{
			LOGE("GL_ERROR (0x%x)", nError);
		}

		int nUniformNum = 0;
		glGetProgramiv(this->_program, GL_ACTIVE_UNIFORMS, &nUniformNum);


		char sVarName[64];
		for (int i = 0; i < nUniformNum; i++) 
		{
			GLenum type;
			int length, size;
			glGetActiveUniform(this->_program, i, sizeof(sVarName), &length, &size, &type, sVarName);
			int location = glGetUniformLocation(this->_program, sVarName);
			/*one = { vartype: "uniform", glfun : null, ivartype : 1, location : location, name : uniform.name, type : uniform.type, isArray : false, isSame : false, preValue : null, indexOfParams : 0 };
			if (one.name.indexOf('[0]') > 0) {
				one.name = one.name.substr(0, one.name.length - 3);
				one.isArray = true;
				one.location = gl.getUniformLocation(this._program, one.name);
			}
			this._params.push(one);*/
			UniformInfo* info = new UniformInfo();
			info->location = location;
			info->name = sVarName;
			info->type = type;
			info->size = size;
			m_uniformInfoMap.insert(std::make_pair(sVarName, info));
		}
	}
	void Shader2D::use()
	{
		m_pRender2DContext->bindUseProgram(this->_program);
	}
	int Shader2D::uniform1f(const char* name, float value)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_FLOAT && info->size == 1);
			if (info->uploadedValue.valueFloat == nullptr || info->uploadedValue.valueFloat[0] != value)
			{
				if (info->uploadedValue.valueFloat == nullptr)
				{
					info->uploadedValue.valueFloat= new float[1];
				}
				glUniform1f(info->location, info->uploadedValue.valueFloat[0] = value);
				return 1;
			}
		}
		return 0;
	}
	int Shader2D::uniform1i(const char* name, int value)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_INT && info->size == 1);
			if (info->uploadedValue.valueInt == nullptr || info->uploadedValue.valueInt[0] != value)
			{
				if (info->uploadedValue.valueInt == nullptr)
				{
					info->uploadedValue.valueInt = new int[1];
				}
				glUniform1i(info->location, info->uploadedValue.valueInt[0] = value);
				return 1;
			}
		}
		return 0;
	}
	int Shader2D::uniform2f(const char* name, float value0, float value1)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_FLOAT_VEC2 && info->size == 1);
			if (info->uploadedValue.valueFloat == nullptr || info->uploadedValue.valueFloat[0] != value0 || info->uploadedValue.valueFloat[1] != value1)
			{
				if (info->uploadedValue.valueFloat == nullptr)
				{
					info->uploadedValue.valueFloat = new float[2];
				}
				glUniform2f(info->location, info->uploadedValue.valueFloat[0] = value0, info->uploadedValue.valueFloat[1] = value1);
				return 1;
			}
		}
		return 0;
	}
	int Shader2D::uniform3f(const char* name, float value0, float value1, float value2)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_FLOAT_VEC3 && info->size == 1);
			if (info->uploadedValue.valueFloat == nullptr || info->uploadedValue.valueFloat[0] != value0 || info->uploadedValue.valueFloat[1] != value1 || info->uploadedValue.valueFloat[2] != value2)
			{
				if (info->uploadedValue.valueFloat == nullptr)
				{
					info->uploadedValue.valueFloat = new float[3];
				}
				glUniform3f(info->location, info->uploadedValue.valueFloat[0] = value0, info->uploadedValue.valueFloat[1] = value1, info->uploadedValue.valueFloat[2] = value2);
				return 1;
			}
		}
		return 0;
	}
	int Shader2D::uniform4f(const char* name, float value0, float value1, float value2, float value3)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_FLOAT_VEC4 && info->size == 1);
			if (info->uploadedValue.valueFloat == nullptr || info->uploadedValue.valueFloat[0] != value0 || info->uploadedValue.valueFloat[1] != value1 || info->uploadedValue.valueFloat[2] != value2 || info->uploadedValue.valueFloat[3] != value3)
			{
				if (info->uploadedValue.valueFloat == nullptr)
				{
					info->uploadedValue.valueFloat = new float[4];
				}
				glUniform4f(info->location, info->uploadedValue.valueFloat[0] = value0, info->uploadedValue.valueFloat[1] = value1, info->uploadedValue.valueFloat[2] = value2, info->uploadedValue.valueFloat[3] = value3);
				return 1;
			}
		}
		return 0;
	}
	int Shader2D::uniform_sampler2D(const char* name, WebGLInternalTex* value)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_SAMPLER_2D && info->size == 1);
			if (info->uploadedValue.valueInt == nullptr)
			{
				info->uploadedValue.valueInt = new int[1];
				info->uploadedValue.valueInt[0] = m_curActTexIndex;
				glUniform1i(info->location, m_curActTexIndex);
				m_pRender2DContext->activeTexture(GL_TEXTURE0 + m_curActTexIndex);
				m_pRender2DContext->bindTexture(GL_TEXTURE_2D, value);

				m_curActTexIndex++;
				return 1;
			}
			else
			{
				m_pRender2DContext->activeTexture(GL_TEXTURE0 + info->uploadedValue.valueInt[0]);
				m_pRender2DContext->bindTexture(GL_TEXTURE_2D, value);
				return 0;
			}
		}
		return 0;
	}
	int Shader2D::uniformMatrix4fv(const char* name, float* value)
	{
		std::unordered_map<std::string, UniformInfo*>::iterator it = m_uniformInfoMap.find(name);
		if (it != m_uniformInfoMap.end())
		{
			UniformInfo* info = it->second;
			assert(info->type == GL_FLOAT_MAT4 && info->size == 1);
			glUniformMatrix4fv(info->location, 1, false, value);
			return 1;

		}
		return 0;
	}
	void Shader2D::recreateResource()
	{
		this->_compile();
		//this->_setGPUMemory(0);//忽略尺寸尺寸
	}

	void Shader2D::_disposeResource()
	{
		glDeleteShader(this->_vshader);
		glDeleteShader(this->_pshader);
		glDeleteProgram(this->_program);
		this->_vshader = this->_pshader = this->_program = 0;
		//this->_params = null;
		//this->_paramsMap = {};
		//this->_setGPUMemory(0);
		//this->_curActTexIndex = 0;
	}
	GLuint Shader2D::_createShader(const std::string& str, int type)
	{
		GLuint shader = glCreateShader(type);
		const char* strShaders[1] = { str.c_str() };
		glShaderSource(shader, 1, strShaders, NULL);
		glCompileShader(shader);

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) 
		{
			GLint infoLen = 512;// 0;
			//glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = new char[infoLen];
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGI("%s", buf);
					delete[] buf;
				}
			}
			return 0;
		}
		return shader;
	}
}
//------------------------------------------------------------------------------

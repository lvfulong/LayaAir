#ifndef __Shader2D_H__
#define __Shader2D_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include <utils/Preprocessor.h>
#include "Shader2DDefines.h"

namespace laya
{
	class WebGLEngine;
	class IRender2DContext;
	class Shader;
	class Shader2DCompile;
	class WebGLInternalTex;
	struct UniformInfo
	{
		std::string name;
		GLenum type;
		int location;
		int size = 1;
		union
		{
			float* valueFloat;
			int* valueInt;
		}uploadedValue;
		~UniformInfo()
		{
			if (uploadedValue.valueFloat != nullptr)
			{
				delete[] uploadedValue.valueFloat;
				uploadedValue.valueFloat = nullptr;
			}

			if (uploadedValue.valueInt != nullptr)
			{
				delete[] uploadedValue.valueInt;
				uploadedValue.valueInt = nullptr;
			}
		}
	};
	//typedef Shader* (*createShaderFunction)(const std::string& vs, const std::string& ps, const std::string& saveName, /*const std::map<int, std::string>& nameMap,*/ const std::vector<std::pair<std::string, int> >& bindAttrib);
	class Shader2D
	{	
	public:
		Shader2D(WebGLEngine* pWebGLEngine, const std::string& vs, const std::string& ps, /*const std::map<int, std::string>& nameMap,*/ const std::vector<std::pair<std::string, int> >& bindAttrib);
		void recreateResource();
		void _disposeResource();
		void _compile();
		static void preprocess2D(/*int nameID, */int mainID, const char* vs, const char* ps/*, nameMap : any*/);
		//static Shader* withCompile2D(/*int nameID,*/ int mainID, const std::vector<std::string>& define, const std::string& shaderName, createShaderFunction createShader, const std::vector<std::pair<std::string, int> >& bindAttrib);
		static Shader2D* withCompile2D(WebGLEngine* pWebGLEngine, int shaderID, const Shader2DDefines& defines, const std::vector<std::pair<std::string, int> >& bindAttrib);
		static GLuint _createShader(const std::string& str, int type);
		//static Shader* create(const std::string& vs, const std::string& ps, const std::string& saveName, /*const std::map<int, std::string>& nameMap,*/ const std::vector<std::pair<std::string, int> >& bindAttrib);
		void use();
		int uniform1f(const char* name, float value);
		int uniform1i(const char* name, int value);
		int uniform2f(const char* name, float value0, float value1);
		int uniform3f(const char* name, float value0, float value1, float value2);
		int uniform4f(const char* name, float value0, float value1, float value2, float value3);
		int uniform_sampler2D(const char* name, WebGLInternalTex* value);
		int uniformMatrix4fv(const char* name, float* value);
        static void onDestroy();
	public:
		std::string _vs;
		std::string _ps;
		GLuint _vshader = 0;
		GLuint _pshader = 0;
		GLuint _program = 0;
		bool _reCompile;
		std::vector<std::pair<std::string, int>> _attribInfo;
		static std::unordered_map<int, Shader2DCompile*> _preCompileShader; //�洢Ԥ������������ͨ�����ֻ������,Ŀǰ��֧��#ifdefǶ�׺�����
		static std::unordered_map<uint64_t, Shader2D*> shaders;
		std::unordered_map<std::string, UniformInfo*> m_uniformInfoMap;
		int m_curActTexIndex = 0;
		IRender2DContext* m_pRender2DContext;
	};
}
#endif //__Shader2D_H__

#ifndef __Shader2DCompile_H__
#define __Shader2DCompile_H__

#include <stdio.h>
#include <string>
#include <regex>
#include <map>
#include "Shader2D.h"

namespace laya
{
	class GLESEngine;
	class ShaderNode;
	class InlcudeFile;
	class Shader2DCompile
	{
	public:
		enum 
		{
			IFDEF_NO = 0,
			IFDEF_YES = 1,
			IFDEF_ELSE = 2,
			IFDEF_PARENT = 3,
		};
		Shader2DCompile(const std::string& vs, const std::string& ps/*, nameMap : any*/);
		void _compileToTree(ShaderNode* parent, std::vector<std::string>& lines, int start/*, includefiles : any[]*/, std::unordered_map<std::string, bool>& defs);
		Shader2D* createShader(GLESEngine* pGLESEngine, const std::vector<std::string>& define, const std::vector<std::pair<std::string, int> >& bindAttrib);
		static void splitToWords(const std::string& str, ShaderNode* block, std::vector<std::string>& out);
	protected:
		ShaderNode* _compile(const std::string& script);
		ShaderNode* _VS;
		ShaderNode* _PS;
		std::unordered_map<std::string, bool> defs;
	public:
		//static std::unordered_map<std::string, InlcudeFile*> includes;
	};
}
#endif //__Shader2DCompile_H__

/**
@file			ShaderNode.h
@brief
@author			James
@version		1.0
@date			2016_5_18
*/

#ifndef __ShaderNode_H__
#define __ShaderNode_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
/**
* @brief
*/
namespace laya
{
	class ShaderNode
	{
	public:
		ShaderNode(/*includefiles: any[]*/);
		void setParent(ShaderNode*parent);
		void setCondition(const std::string& condition, int type);
		void toscript(const std::map<std::string, bool>& def, std::vector<std::string>& out);
		void _toscript(const std::map<std::string, bool>& def, std::vector<std::string>& out, int id);
		std::vector<ShaderNode*> childs;
		std::string text = "";
		ShaderNode* parent = nullptr;
		std::string name;
		bool noCompile = true;
		std::string condition;
		int conditionType;
		int z = 0;
		std::string src;
		std::string useFuns = "";
		static int __id;
	};
}
//------------------------------------------------------------------------------


#endif //__ShaderNode_H__

//-----------------------------END FILE--------------------------------
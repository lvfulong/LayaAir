#include "Shader2DNode.h"
#include <regex>
#include "Shader2DCompile.h"

namespace laya
{
	static std::regex _regex("(\s*$)", std::regex::ECMAScript);
	int ShaderNode::__id = 1;
	ShaderNode::ShaderNode(/*includefiles: any[]*/)
	{

	}
	void ShaderNode::setParent(ShaderNode*parent)
	{
		parent->childs.push_back(this);
		this->z = parent->z + 1;
		this->parent = parent;
	}
	void ShaderNode::setCondition(const std::string& condition, int type)
	{
		//if (condition) {
			this->conditionType = type;
			this->condition = std::regex_replace(condition, _regex, "", std::regex_constants::match_any);
		//}
	}
	void ShaderNode::toscript(const std::map<std::string, bool>& def, std::vector<std::string>& out)
	{
		_toscript(def, out, ++ShaderNode::__id);
	}
	void ShaderNode::_toscript(const std::map<std::string, bool>& def, std::vector<std::string>& out, int id)
	{
		if (this->childs.size() < 1 && this->text == "") return;
		int outIndex = out.size();
		if (this->condition != "")
		{
			const std::map<std::string, bool>::const_iterator it = def.find(this->condition);
			bool ifdef = (it != def.end()) && it->second;
			(this->conditionType == Shader2DCompile::IFDEF_ELSE) && (ifdef = !ifdef);
			if (!ifdef) return;
		}
		if (this->noCompile)
		{
			if (this->text != "")
			{
				out.push_back(this->text);
			}
		}
		if (this->childs.size() > 0)
		{
			for (int i = 0; i < this->childs.size(); i++)
			{
				this->childs[i]->_toscript(def, out, id);
			}
		}

	/*if (this.includefiles.length > 0 && this.useFuns.length > 0) {
		var funsCode : string;
		for (var i : number = 0, n : number = this.include files.length; i < n; i++) {
			//如果已经加入了，就不要再加
			if (this.includefiles[i].curUseID == id) {
				continue;
			}
			funsCode = this.includefiles[i].file.getFunsScript(this.useFuns);
			if (funsCode.length > 0) {
				this.includefiles[i].curUseID = id;
				out[0] = funsCode + out[0];
			}
		}
	}*/
	}
}
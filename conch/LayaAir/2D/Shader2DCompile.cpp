#include "Shader2DCompile.h"
#include "Shader2DNode.h"
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include "Shader2D.h"

namespace laya
{
	static std::regex _newline("\\n", std::regex::ECMAScript);
	static std::regex _clearCR("\\r", std::regex::ECMAScript);
	static std::regex _splitToWordExps3("[ \\t=\\+\\-*/&%!<>!%\(\),;\\|]", std::regex::ECMAScript);
	/*static void split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ")
	{
		std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
		std::string::size_type pos = s.find_first_of(delimiters, lastPos);
		while (std::string::npos != pos || std::string::npos != lastPos) 
		{
			tokens.emplace_back(s.substr(lastPos, pos - lastPos));
			lastPos = s.find_first_not_of(delimiters, pos);
			pos = s.find_first_of(delimiters, lastPos);
		}
	}*/
	Shader2DCompile::Shader2DCompile(const std::string& vs, const std::string& ps/*, nameMap : any*/)
	{
		//LOGI("vs %s", vs.c_str());
		//LOGI("ps %s", ps.c_str());
		this->_VS = _compile(vs);
		this->_PS = _compile(ps);
	}
	ShaderNode* Shader2DCompile::_compile(const std::string& script)
	{
		std::string new_script = std::regex_replace(script, _clearCR, "", std::regex_constants::match_any);//CRLF�����Ҫ��ȥ����\r",�����з��ַ�������º궨���������
		//LOGI("Shader2DCompile::_compile %d  %s ", std::string::npos, new_script.c_str());
		//var includefiles : any[] = [];
		ShaderNode* top  = new ShaderNode(/*includefiles*/);
		std::vector<std::string> split_script;
		split(new_script, split_script, "\n");
		this->_compileToTree(top, split_script, 0/*, includefiles*/, this->defs);
		return top;
	}
	void Shader2DCompile::_compileToTree(ShaderNode* parent, std::vector<std::string>& lines, int start/*, includefiles : any[]*/, std::unordered_map<std::string, bool>& defs)
	{
		ShaderNode* node = nullptr;
		ShaderNode* preNode = nullptr;
		ShaderNode* noUseNode = nullptr;
		std::string name;
		int i;
		int n;
		int j;
		
		for (i = start; i < lines.size(); i++)
		{
			std::string& text = lines[i];
			if (text.size() < 1)
				continue;
			//int ofs = text.find_first_of("////");
			int ofs = text.find("//");
			if (ofs == 0)
				continue;
			if (ofs > 0)
			{
				text = text.substr(0, ofs);
			}
			node = noUseNode != nullptr ? noUseNode : new ShaderNode(/*includefiles*/);
			noUseNode = nullptr;
			node->text = text;
			node->noCompile = true;
			
			if ((ofs = text.find_first_of("#")) >= 0) {
				name = "#";
				for (j = ofs + 1, n = text.size(); j < n; j++)
				{
					char c = text[j];
					if (c == ' ' || c == '\t' || c == '?') break;
					name += c;
				}
				node->name = name;
				if (name == "#ifdef" || name == "#ifndef")
				{
					node->src = text;
					node->noCompile = std::regex_match(text, std::regex("[!&|()=<>]", std::regex::ECMAScript));
					if (!node->noCompile)
					{
						std::string replace_text = std::regex_replace(text, std::regex("^\\s*", std::regex::ECMAScript), "", std::regex_constants::match_any);
						//std::regex re("\\s+", std::regex::ECMAScript);
						//std::regex re("\\s+");
						//auto re = std::regex(R"(\s+)", std::regex::optimize);
						//std::vector<std::string> words(std::sregex_token_iterator(replace_text.begin(), replace_text.end(), re, -1), std::sregex_token_iterator());
						std::vector<std::string> words;
						//std::copy(std::sregex_token_iterator(replace_text.begin(), replace_text.end(), re, -1), std::sregex_token_iterator(), std::back_inserter(words));

						split(replace_text, words, " \t\n\v\f\r");
						node->setCondition(words[1], name == "#ifdef" ? Shader2DCompile::IFDEF_YES : Shader2DCompile::IFDEF_ELSE);
						node->text = "//" + node->text;
					}
					else
					{
						//console.log("function():Boolean{return " + text.substr(ofs + node.name.length) + "}");
					}
					node->setParent(parent);
					parent = node;
					//if (defs) {
						std::string sub_text = text.substr(j);
						std::vector<std::string> words(std::regex_token_iterator<std::string::iterator>(sub_text.begin(), sub_text.end(), _splitToWordExps3, -1), std::regex_token_iterator<std::string::iterator>());
						for (j = 0; j < words.size(); j++)
						{
							text = words[j];
							!text.empty() && (defs[text] = true);
						}
					//}
					continue;
				}
				else if (name == "#if")
				{
					node->src = text;
					node->noCompile = true;
					node->setParent(parent);
					parent = node;
					//if (defs) {
						std::string sub_text = text.substr(j);
						std::vector<std::string> words(std::sregex_token_iterator(sub_text.begin(), sub_text.end(), _splitToWordExps3, -1), std::sregex_token_iterator());
						for (j = 0; j < words.size(); j++) 
						{
							text = words[j];
							!text.empty() && text != "defined" && (defs[text] = true);
						}
					//}
					continue;
				}
				else if (name == "#else")
				{
					node->src = text;
					parent = parent->parent;
					preNode = parent->childs[parent->childs.size() - 1];
					node->noCompile = preNode->noCompile;
					if (!node->noCompile) {
						node->condition = preNode->condition;
						node->conditionType = preNode->conditionType == Shader2DCompile::IFDEF_YES ? Shader2DCompile::IFDEF_ELSE : Shader2DCompile::IFDEF_YES;
						node->text = "//" + node->text + " " + preNode->text + " " + std::to_string(node->conditionType);
					}
					node->setParent(parent);
					parent = node;
					continue;
				}
				else if (name == "#endif")
				{
					parent = parent->parent;
					preNode = parent->childs[parent->childs.size() - 1];
					node->noCompile = preNode->noCompile;
					if (!node->noCompile) {
						node->text = "//" + node->text;
					}
					node->setParent(parent);
					continue;
				}
				else if (name == "#include")//����������,��Ҫ�ǿո�
				{
					/*std::vector<std::string> words;
					Shader2DCompile::splitToWords(text, nullptr, words);
					InlcudeFile* inlcudeFile = Shader2DCompile::includes[words[1]];
					if (!inlcudeFile) 
					{
						LOGE("Shader2DCompile error no this include file:%s", words[1]);
						return;
					}
					if ((ofs = words[0].find_first_of("?")) < 0) 
					{
						node->setParent(parent);
						text = inlcudeFile->getWith(words[2] == "with" ? words[3] : nullptr);
						std::vector<std::string> text_split(std::sregex_token_iterator(text.begin(), text.end(), _newline, -1), std::sregex_token_iterator());
						this->_compileToTree(node, text_split, 0, includefiles, defs);
						node->text = "";
						continue;
					}
					node->setCondition(words[0].substr(ofs + 1), Shader2DCompile::IFDEF_YES);
					node->text = inlcudeFile->getWith(words[2] == "with" ? words[3] : nullptr);*/
				}
				else if (name == "#import")
				{
					//TODO
					continue;
				}
			}
			else 
			{
				if (!parent->childs.empty())
				{
					preNode = parent->childs[parent->childs.size() - 1];
					if (preNode && preNode->name == "")
					{
						//includefiles.length > 0 && Shader2DCompile::splitToWords(text, preNode);
						noUseNode = node;
						preNode->text += "\n" + text;
						continue;
					}
					//includefiles.length > 0 && Shader2DCompile::splitToWords(text, node);
				}
			}
			node->setParent(parent);
		}
	}
	void Shader2DCompile::splitToWords(const std::string& str, ShaderNode* block, std::vector<std::string>& out)
	{
		static std::string STRING = " \t=+-*/&%!<>()'\",;";
		static std::string STRING1 = "vec4;main;";
		char c;
		int ofs = -1;
		std::string word;
		int n;
		for (int i = 0, n = str.size(); i < n; i++)
		{
			c = str[i];
			if (STRING.find_first_of(c) >= 0) 
			{
				if (ofs >= 0 && (i - ofs) > 1) 
				{
					word = str.substr(ofs, i - ofs);
					out.push_back(word);
				}
				if (c == '"' || c == '\'')
				{
					int ofs2 = str.find_first_of(c, i + 1);
					if (ofs2 < 0)
					{
						LOGE("Sharder err:%s", str.c_str());
						return;
					}
					out.push_back(str.substr(i + 1, ofs2 - i - 1));
					i = ofs2;
					ofs = -1;
					continue;
				}
				if (c == '(' && block && out.size() > 0)
				{
					word = out[out.size() - 1] + ";";
					if (STRING1.find_first_of(word) < 0)
						block->useFuns += word;
				}
				ofs = -1;
				continue;
			}
			if (ofs < 0) ofs = i;
		}
		if (ofs < n && (n - ofs) > 1) 
		{
			word = str.substr(ofs, n - ofs);
			out.push_back(word);
		}
	}
	Shader2D* Shader2DCompile::createShader(WebGLEngine* pWebGLEngine, const std::vector<std::string>& define, const std::vector<std::pair<std::string, int> >& bindAttrib)
	{
		std::map<std::string, bool> defMap;
		std::string defineStr = "";
		//if (define) 
		{
			for (std::vector<std::string>::const_iterator it = define.begin(); it != define.end(); it++)
			{
				defineStr += std::string("#define ") + *it + "\n";
				defMap[*it] = true;
			}
		}
		std::vector<std::string> vs;
		std::vector<std::string> ps;
		this->_VS->toscript(defMap, vs);
		this->_PS->toscript(defMap, ps);
		std::string strVs;
		std::string strPs;
		for (int i = 0; i < vs.size(); i++)
		{
			strVs += (vs[i] + "\n");
		}
		for (int i = 0; i < ps.size(); i++)
		{
			strPs += (ps[i] + "\n");
		}
		return new Shader2D(pWebGLEngine, defineStr + strVs, defineStr + strPs/*, this->_nameMap*/, bindAttrib);
	}
}
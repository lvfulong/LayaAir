#ifndef __CommandEncoder_H__
#define __CommandEncoder_H__

#include <stdio.h>
#include <vector>

namespace laya
{
	class ShaderVariable;
	class CommandEncoder
	{
	public:
		CommandEncoder() {}
		~CommandEncoder()
		{
			m_idata.clear();
		}
		void addShaderUniform(ShaderVariable* variable)
		{
			m_idata.push_back(variable);
		}
		std::vector<ShaderVariable*>& getArrayData()
		{
			return m_idata;
		}
		void clear()
		{
			m_idata.clear();
		}
	public:
		std::vector<ShaderVariable*> m_idata;
	};
}
#endif //__CommandEncoder_H__

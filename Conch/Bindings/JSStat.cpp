#include "JSStat.h"


namespace laya 
{
	JSStat::~JSStat()
	{
	}
	bool JSStat::isDirectory()
	{
		return m_isDirectory;
	}
	bool JSStat::isFile()
	{
		return m_isFile;
	}
	int JSStat::getSize()
	{
		return m_nSize;
	}
    void JSStat::exportJS(Context& context)
    {
		class_<JSStat> class_binding;
		class_binding.constructor<>();
		class_binding.property("size", &JSStat::getSize);
		class_binding.function("isDirectory", &JSStat::isDirectory);
		class_binding.function("isFile", &JSStat::isFile);
		context.class_("_stat", class_binding);
    }
}

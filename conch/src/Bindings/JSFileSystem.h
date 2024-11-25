#ifndef __JSFileSystem_H__
#define __JSFileSystem_H__

#include <jsbind/JSBind.h>

namespace laya
{
	class JSFileSystem
    {
	public:
		static bool rmDir(const char* p_pszPath, jsvm_value onprogress, jsvm_value oncomplete, jsvm_value onerror);
		static bool rmDirSync(const char* p_pszPath);
		static jsvm_value readBinFileSync(const char* p_pszFile);
		static jsvm_value readdirSync(const std::string &path);
		static jsvm_value lstatSync(const std::string &path);
        static bool JSWriteFileSync(const char* p_sUrl, jsbind::Local args);
	};
}
#endif 
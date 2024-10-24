#ifndef __JSFileSystem_H__
#define __JSFileSystem_H__

#include <binder/JSInterface.h>

namespace laya
{
	class JSFileSystem
    {
	public:
		static bool rmDir(const char* p_pszPath, JSValueAsParam onprogress, JSValueAsParam oncomplete, JSValueAsParam onerror);
		static bool rmDirSync(const char* p_pszPath);
		static JsValue readBinFileSync(const char* p_pszFile);
		static JsValue readdirSync(const std::string &path);
		static JsValue lstatSync(const std::string &path);
        static bool JSWriteFileSync(const char* p_sUrl, jsbind::Local args);
	};
}
#endif 
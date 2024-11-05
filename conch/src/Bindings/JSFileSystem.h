#ifndef __JSFileSystem_H__
#define __JSFileSystem_H__

#include <binder/JSBind.h>

namespace laya
{
	class JSFileSystem
    {
	public:
		static bool rmDir(const char* p_pszPath, jsvm::Value onprogress, jsvm::Value oncomplete, jsvm::Value onerror);
		static bool rmDirSync(const char* p_pszPath);
		static jsvm::Value readBinFileSync(const char* p_pszFile);
		static jsvm::Value readdirSync(const std::string &path);
		static jsvm::Value lstatSync(const std::string &path);
        static bool JSWriteFileSync(const char* p_sUrl, jsbind::Local args);
	};
}
#endif 
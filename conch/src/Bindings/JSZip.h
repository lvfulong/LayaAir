#ifndef __JSZip_H__
#define __JSZip_H__

#include <jsbind/JSBind.h>
#include <zip/JCZip.h>

namespace laya 
{
	class JSArrayBuffer;
	class JSZip
    {
	public:
		static void exportJS(jsbind::Object& context);
		JSZip();
		~JSZip();
		bool openZip(const char* p_pszFile);
		void enumFiles(jsbind::Local callback);
		bool setSrc(const char* p_pSrc);
        bool setBuffer(jsbind::ArrayBuffer arrayBuffer);
		jsvm::Value readFile(int idx);
        jsvm::Value readFileByName(const char* pName);
        jsvm::Value readFileAsText(int idx);
        jsvm::Value readFileAsTextByName(const char* pName);
		jsvm::Value readFileExp(int idx);
        bool exists(const char* pName);
		void close();
	public:
		JCZip m_zip;
	};
}
#endif
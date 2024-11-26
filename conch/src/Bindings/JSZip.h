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
		jsvm_value readFile(int idx);
        jsvm_value readFileByName(const char* pName);
        jsvm_value readFileAsText(int idx);
        jsvm_value readFileAsTextByName(const char* pName);
		jsvm_value readFileExp(int idx);
        bool exists(const char* pName);
		void close();
	public:
		JCZip m_zip;
	};
}
#endif
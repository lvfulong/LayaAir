#ifndef __JSZip_H__
#define __JSZip_H__

#include <binder/JSInterface.h>
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
		void enumFiles( JSValueAsParam pObj);
		bool setSrc(const char* p_pSrc);
        bool setBuffer(jsbind::ArrayBuffer arrayBuffer);
		JsValue readFile(int idx);
        JsValue readFileByName(const char* pName);
        JsValue readFileAsText(int idx);
        JsValue readFileAsTextByName(const char* pName);
		JsValue readFileExp(int idx);
        bool exists(const char* pName);
		void close();
	public:
		JCZip m_zip;
	};
}
#endif
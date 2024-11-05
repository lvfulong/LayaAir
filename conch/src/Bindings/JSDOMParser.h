#ifndef __JSDOMParser_H__
#define __JSDOMParser_H__

#include <stdio.h>
#include <binder/JSBind.h>
#include "resource/JCFileResManager.h"
#include "JSXmlNode.h"

namespace laya
{
	class JSDOMParser
	{
	public:
		static void exportJS(jsbind::Object& context);
		JSDOMParser();
		~JSDOMParser();
		jsvm::Value parseFromString(const char * str,const char *type);
		const char* getSrc();
		void setSrc( const char* p_sSrc );
		void SetOnload( jsvm::Value p_pFunction );
		jsvm::Value GetOnload();
		void SetOnError( jsvm::Value p_pFunction );
		jsvm::Value GetOnError();
		jsvm::Value getXml();
	public:
		void onLoadedCallJSFunction(std::string& str,std::weak_ptr<int>& callbackref);
		void onErrorCallJSFunction( int e , std::weak_ptr<int>& callbackref);
	public:

		std::shared_ptr<int> m_CallbackRef;

	public:
		std::string		m_sUrl;
		jsbind::Persistent		m_pOnLoadJSFunction;
		jsbind::Persistent		m_pOnErrorJSFunction;
	private:
		JSXmlDocument* jsDOC;
	};
}

#endif
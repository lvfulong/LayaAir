#ifndef __JSGlobalExportCFun_H__
#define __JSGlobalExportCFun_H__


  #include <jsbind/JSBind.h>
 
namespace laya 
{
	
    void JSPrint( const char* p_sBuffer );

    void LayaAlert(const char* p_sBuffer);

	void JSAlert( const char* p_sBuffer );

	void evalJS( const char* p_sSource );

	void JSGlobalExportC();

    void JSGlobalDisExportC();

    std::string conchToBase64(const char* type, float encoderOptions, jsbind::ArrayBuffer ab, int w, int h);

    std::string conchToBase64FlipY(const char* type, float encoderOptions, jsbind::ArrayBuffer ab, int w, int h);

}
#endif
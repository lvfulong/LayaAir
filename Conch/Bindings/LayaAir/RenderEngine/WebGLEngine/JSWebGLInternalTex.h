#ifndef __JSWebGLInternalTex_H__
#define __JSWebGLInternalTex_H__

#include <Binder/JSInterface.h>

namespace laya
{
	class JCImage;
	class WebGLInternalTex;
	class JSWebGLInternalTex
	{
	public:
		static void exportJS(Context& context);
		JSWebGLInternalTex();
		JSWebGLInternalTex(std::shared_ptr<WebGLInternalTex> texture);
		~JSWebGLInternalTex();
		void setCompareMode(int mode);
		int getCompareMode();
		void setAnisoLevel(float value);
		float getAnisoLevel();
		int getWrapU();
		void setWrapU(int value);
		int getWrapV();
		void setWrapV(int value);
		int getWrapW();
		void setWrapW(int value);
		int getFilterMode();
		void setFilterMode(int value);
		void dispose();
		int getID();
		int getResource();
		int getGammaCorrection();
		void setGammaCorrection(int gammaCorrection);
		bool getUseSRGBLoad();
		bool getMipmap();
		int getMipmapCount();
		void setBaseMipmapLevel(int value);
		int getBaseMipmapLevel();
		void setMaxMipmapLevel(int value);
		int getMaxMipmapLevel();
        int getInternalFormat();
        void setInternalFormat(int value);
	public:
        std::shared_ptr<WebGLInternalTex>		m_texture;
	};
}
#endif //__JSWebGLInternalTex_H__

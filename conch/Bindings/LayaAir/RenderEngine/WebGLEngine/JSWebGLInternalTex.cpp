#include "JSWebGLInternalTex.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#include <render/driver/gles/WebGLInternalTex.h>

namespace laya
{
	JSWebGLInternalTex::JSWebGLInternalTex()
	{
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLInternalTex", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLInternalTex::JSWebGLInternalTex(std::shared_ptr<WebGLInternalTex> texture)
	{
		m_texture = texture;
		AdjustAmountOfExternalAllocatedMemory(4);
		JCMemorySurvey::GetInstance()->newClass("conchWebGLInternalTex", 4, this);
	}
	//------------------------------------------------------------------------------
	JSWebGLInternalTex::~JSWebGLInternalTex()
	{
        m_texture.reset();
		JCMemorySurvey::GetInstance()->releaseClass("conchWebGLInternalTex", this);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getID()
	{
		return m_texture->getID();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setCompareMode(int mode)
	{
		m_texture->setCompareMode((TextureCompareMode)mode);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getCompareMode()
	{
		return (int)m_texture->getCompareMode();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setAnisoLevel(float value)
	{
		m_texture->setAnisoLevel(value);
	}
	//------------------------------------------------------------------------------
	float JSWebGLInternalTex::getAnisoLevel()
	{
		return m_texture->getAnisoLevel();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::dispose()
	{
		return m_texture->dispose();
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getWrapU()
	{
		return (int)m_texture->getWrapU();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setWrapU(int value)
	{
		m_texture->setWrapU((WarpMode)value);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getWrapV()
	{
		return (int)m_texture->getWrapV();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setWrapV(int value)
	{
		m_texture->setWrapV((WarpMode)value);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getWrapW()
	{
		return (int)m_texture->getWrapW();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setWrapW(int value)
	{
		m_texture->setWrapW((WarpMode)value);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getFilterMode()
	{
		return (int)m_texture->getFilterMode();
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setFilterMode(int value)
	{
		m_texture->setFilterMode((FilterMode)value);
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getResource()
	{
		return m_texture->getResource();
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getGammaCorrection()
	{
		return m_texture->m_gammaCorrection;
	}
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::setGammaCorrection(int gammaCorrection)
	{
		m_texture->m_gammaCorrection = gammaCorrection;
	}
	//------------------------------------------------------------------------------
	bool JSWebGLInternalTex::getUseSRGBLoad()
	{
		return m_texture->m_useSRGBLoad;
	}
	//------------------------------------------------------------------------------
	bool JSWebGLInternalTex::getMipmap()
	{
		return m_texture->m_mipmap;
	}
	//------------------------------------------------------------------------------
	int JSWebGLInternalTex::getMipmapCount()
	{
		return m_texture->m_mipmapCount;
	}
	void JSWebGLInternalTex::setBaseMipmapLevel(int value)
	{
		m_texture->setBaseMipmapLevel(value);
	}

	int JSWebGLInternalTex::getBaseMipmapLevel()
	{
		return m_texture->getBaseMipmapLevel();
	}

	void JSWebGLInternalTex::setMaxMipmapLevel(int value)
	{
		m_texture->setMaxMipmapLevel(value);
	}

	int JSWebGLInternalTex::getMaxMipmapLevel()
	{
		return m_texture->getMaxMipmapLevel();
	}
    int JSWebGLInternalTex::getInternalFormat()
    {
        return m_texture->getInternalFormat();
    }
    void JSWebGLInternalTex::setInternalFormat(int value)
    {
        return m_texture->setInternalFormat(value);
    }
	//------------------------------------------------------------------------------
	void JSWebGLInternalTex::exportJS(Context& context)
	{
        class_<JSWebGLInternalTex> class_binding;
		class_binding.constructor<>();
        class_binding.property("mipmapCount", &JSWebGLInternalTex::getMipmapCount);
        class_binding.property("mipmap", &JSWebGLInternalTex::getMipmap);
        class_binding.property("useSRGBLoad", &JSWebGLInternalTex::getUseSRGBLoad);
        class_binding.property("gammaCorrection", &JSWebGLInternalTex::getGammaCorrection, &JSWebGLInternalTex::setGammaCorrection);
        class_binding.property("resource", &JSWebGLInternalTex::getResource);
        class_binding.property("id", &JSWebGLInternalTex::getID);
        class_binding.property("filterMode", &JSWebGLInternalTex::getFilterMode, &JSWebGLInternalTex::setFilterMode);
        class_binding.property("wrapU", &JSWebGLInternalTex::getWrapU, &JSWebGLInternalTex::setWrapU);
        class_binding.property("wrapV", &JSWebGLInternalTex::getWrapV, &JSWebGLInternalTex::setWrapV);
        class_binding.property("wrapW", &JSWebGLInternalTex::getWrapW, &JSWebGLInternalTex::setWrapW);
        class_binding.property("anisoLevel", &JSWebGLInternalTex::getAnisoLevel, &JSWebGLInternalTex::setAnisoLevel);
        class_binding.property("compareMode", &JSWebGLInternalTex::getCompareMode, &JSWebGLInternalTex::setCompareMode);
        class_binding.function("dispose", &JSWebGLInternalTex::dispose);
        class_binding.property("maxMipmapLevel", &JSWebGLInternalTex::getMaxMipmapLevel, &JSWebGLInternalTex::setMaxMipmapLevel);
        class_binding.property("baseMipmapLevel", &JSWebGLInternalTex::getBaseMipmapLevel, &JSWebGLInternalTex::setBaseMipmapLevel);
        class_binding.property("internalFormat", &JSWebGLInternalTex::getInternalFormat, &JSWebGLInternalTex::setInternalFormat);
		context.class_("conchWebGLInternalTex", class_binding);
	}
	//------------------------------------------------------------------------------
}

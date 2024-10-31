#include "JSZip.h"
#include <utils/Log.h>
#include <utils/JCCommonMethod.h>
#include <zip.h>

namespace laya 
{
	JSZip::JSZip() 
    {
	}
	JSZip::~JSZip() 
    {
        m_zip.close();
	}
	bool JSZip::openZip(const char* p_pszFile) 
    {
		return m_zip.openZip(p_pszFile);
	}
	bool JSZip::setSrc(const char* p_pSrc) 
    {
		m_zip.close();
		return openZip(p_pSrc);
	}
	JsValue JSZip::readFileExp(int idx)
    {
		return readFile(idx);
	}
	JsValue JSZip::readFile(int idx) 
    {
		if (!m_zip.m_pZip)
			return jsbind::Local::MakeNull();
		zip* pZip = (zip*)m_zip.m_pZip;
		struct zip_stat sb;
		zip_stat_init(&sb);
		if (zip_stat_index(pZip, idx, 0, &sb) < 0)
        {
		}
        else 
        {
			zip_file* pzf = zip_fopen_index(pZip, idx, 0);
			if (pzf == NULL) 
            {
				int a = 0;
			}
			else 
            {
				//laya::JSArrayBuffer* pAB = laya::JSArrayBuffer::create(sb.size);
				char* pBuff = new char[(unsigned int)sb.size];
				zip_fread(pzf, pBuff, sb.size);
				zip_fclose(pzf);
				JsValue ab = jsbind::ArrayBuffer::MakeArrayBuffer((uint8_t*)pBuff,(int)sb.size).getHandle();
				delete [] pBuff;
				//return pAB;
				return ab;
			}
		}
		return jsbind::Local::MakeNull();
	}
    JsValue JSZip::readFileByName(const char* pName)
    {
        if (!pName)
            return jsbind::Local::MakeNull();

        if (!m_zip.m_pZip)
            return jsbind::Local::MakeNull();

        zip* pZip = (zip*)m_zip.m_pZip;
        zip_int64_t idx = zip_name_locate(pZip, pName, 0);

        if (idx == -1)
            return jsbind::Local::MakeNull();

        return readFile(idx);
    }
    JsValue JSZip::readFileAsText(int idx)
    {
        if (!m_zip.m_pZip)
            return jsbind::Local::MakeNull();
        zip* pZip = (zip*)m_zip.m_pZip;
        struct zip_stat sb;
        zip_stat_init(&sb);
        if (zip_stat_index(pZip, idx, 0, &sb) < 0)
        {
        }
        else
        {
            zip_file* pzf = zip_fopen_index(pZip, idx, 0);
            if (pzf == NULL)
            {
                int a = 0;
            }
            else
            {
                std::unique_ptr<char[]> pBuff(new char[(unsigned int)sb.size + 1]);
                zip_fread(pzf, pBuff.get(), sb.size);
                zip_fclose(pzf);
                pBuff[(unsigned int)sb.size] = 0;
#ifdef _DEBUG
                if (IsTextUTF8((char*)pBuff.get(), (unsigned int)sb.size) == false)
                {
                    const char* pName = zip_get_name(pZip, idx, 0);
                    LOGE("error JSZip readFileAsText %s is not utf8 format", pName);
                }
#endif
                jsbind::Local::Make<std::string>(pBuff.get());
            }
        }
        return jsbind::Local::MakeNull();
    }
    JsValue JSZip::readFileAsTextByName(const char* pName)
    {
        if (!pName)
            return jsbind::Local::MakeNull();

        if (!m_zip.m_pZip)
            return jsbind::Local::MakeNull();

        zip* pZip = (zip*)m_zip.m_pZip;
        zip_int64_t idx = zip_name_locate(pZip, pName, 0);

        if (idx == -1)
            return jsbind::Local::MakeNull();

        return readFileAsText(idx);
    }
	void JSZip::enumFiles(jsbind::Local callback)
    {
		if (!m_zip.m_pZip)
			return;
		zip* pZip = (zip*)m_zip.m_pZip;
		int entNum = zip_get_num_files(pZip);
		for (int i = 0; i < entNum; i++)
        {
			struct zip_stat sb;
			zip_stat_init(&sb);
			if (zip_stat_index(pZip, i, 0, &sb) < 0)
            {
			}
			else 
            {
				bool bDir = sb.name[strlen(sb.name) - 1] == '/';
                callback.call<JSZip, void>(this, i, sb.name, bDir, sb.size);
			}
		}
		//zip_close(pZip);
	}
	void JSZip::close()
    {
		m_zip.close();
	}
    bool JSZip::setBuffer(jsbind::ArrayBuffer arrayBuffer)
    {
        DEBUG_CHECK(arrayBuffer.isValid());
        char* pArrayBufferPtr = reinterpret_cast<char*>(arrayBuffer.getData());
        int nABLen = arrayBuffer.getLength();

        if (nABLen <= 0)
        {
            return false;
        }

        close();
        char* pBuff = new char[nABLen];
        memcpy(pBuff, pArrayBufferPtr, nABLen);
        zip_error_t error;
        zip_error_init(&error);
		m_zip.m_pZipSource = zip_source_buffer_create(pBuff, nABLen, 1, &error);

        if (m_zip.m_pZipSource == NULL)
        {
            LOGE("JSZip can't create source: %s", zip_error_strerror(&error));
            zip_error_fini(&error);
            delete[] pBuff;
            return false;
        }

		m_zip.m_pZip = zip_open_from_source((zip_source*)m_zip.m_pZipSource, 0, &error);
        if (!m_zip.m_pZip)
        {
            LOGE("JSZip can't open zip from source: %s", zip_error_strerror(&error));
            zip_source_free((zip_source*)m_zip.m_pZipSource);
            zip_error_fini(&error);
            return false;
        }
        zip_error_fini(&error);
        return true;
    }
    bool JSZip::exists(const char* pName)
    {
        return m_zip.exists(pName);
    }
	void JSZip::exportJS(jsbind::Object& context) 
    {
        jsbind::class_<JSZip> class_binding;
        class_binding.constructor<>();
		class_binding.function("setSrc", &JSZip::setSrc);
		class_binding.function("forEach", &JSZip::enumFiles);
		class_binding.function("readFile1", &JSZip::readFileExp);//这个现在有问题,在导出函数中调用这个无法多次调用。例如enumFiles中调用,只有一次有效。
		class_binding.function("readFile", &JSZip::readFile);
        class_binding.function("readAsArrayBufferByID", &JSZip::readFile);
        class_binding.function("readAsArrayBufferByName", &JSZip::readFileByName);
        class_binding.function("readAsTextByID", &JSZip::readFileAsText);
        class_binding.function("readAsTextByName", &JSZip::readFileAsTextByName);
		class_binding.function("close", &JSZip::close);
        class_binding.function("setBuffer", &JSZip::setBuffer);
        context.class_("ZipFile", class_binding);
	}
}

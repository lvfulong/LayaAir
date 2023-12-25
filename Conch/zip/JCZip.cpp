/**
@file			JCZip.cpp
@brief			
@author			James
@version		1.0
@date			2016_5_18
*/

#include "JCZip.h"
#include <utils/Log.h>
#include <zip.h>
#include <utils/JCBuffer.h>

namespace laya 
{
	JCZip::JCZip()
    {
		m_pZip = NULL;
        m_pZipSource = NULL;
	}
	JCZip::~JCZip()
    {
        close();
	}
	bool JCZip::openZip(const char* p_pszFile)
    {
		int error =0;
		zip* pZip = zip_open(p_pszFile, 0, &error);
		if (!pZip) 
        {
            LOGE("JSZip can't open zip: %d", error);
			return false;
		}
		m_pZip = pZip;
		return true;
	}
	bool JCZip::setSrc(const char* p_pSrc)
    {
        close();
		return openZip(p_pSrc);
	}
	bool JCZip::readFile(int idx, JCBuffer& p_buf)
	{
		if (!m_pZip)
			return false;
		zip* pZip = (zip*)m_pZip;
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
				p_buf.create(sb.size);
				zip_fread(pzf, p_buf.m_pPtr, sb.size);
				zip_fclose(pzf);
				return true;
			}
		}
		return false;
	}
	bool JCZip::readFileAsText(int idx, JCBuffer& p_buf)
	{
		if (!m_pZip)
			return false;
		zip* pZip = (zip*)m_pZip;
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
				p_buf.create(sb.size + 1);
				zip_fread(pzf, p_buf.m_pPtr, sb.size);
				zip_fclose(pzf);
				p_buf.m_pPtr[(unsigned int)sb.size] = 0;
				return true;
			}
		}
		return false;
	}
	bool JCZip::readFileByName(const char* pName, JCBuffer& p_buf)
	{
		if (!pName)
			return false;

		if (!m_pZip)
			return false;

		zip* pZip = (zip*)m_pZip;
		zip_int64_t idx = zip_name_locate(pZip, pName, 0);

		if (idx == -1)
			return false;

		return readFile(idx, p_buf);
	}
	bool JCZip::readFileAsTextByName(const char* pName, JCBuffer& p_buf)
	{
		if (!pName)
			return false;

		if (!m_pZip)
			return false;

		zip* pZip = (zip*)m_pZip;
		zip_int64_t idx = zip_name_locate(pZip, pName, 0);

		if (idx == -1)
			return false;

		return readFileAsText(idx, p_buf);
	}
	void JCZip::close()
    {
        if (m_pZip)
        {
            zip_close((zip*)m_pZip);
        }
        m_pZip = NULL;
        m_pZipSource = NULL;
	}
    bool JCZip::exists(const char* pName)
    {  
		if (!m_pZip)
            return false;

		zip* pZip = (zip*)m_pZip;
		zip_int64_t idx = zip_name_locate(pZip, pName, 0);

		if (idx == -1)
			return false;

        struct zip_stat sb;
        zip_stat_init(&sb);
        if (zip_stat_index(pZip, idx, 0, &sb) < 0)
        {
            return false;
        }
        return true;
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

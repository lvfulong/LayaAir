/**
@file			JCMp3Parser.cpp
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

//包含头文件
#include "JCMp3Parser.h"
#include <utils/Log.h>
#include <vector>

namespace laya
{
//------------------------------------------------------------------------------
JCMp3Parser* JCMp3Parser::ms_pMp3Parser = NULL;
//------------------------------------------------------------------------------
JCMp3Parser::JCMp3Parser()
{
    int error = mpg123_init();
    if (error != MPG123_OK)
    {
        LOGE("mgp123 init error %s", mpg123_plain_strerror(error));
    }
}
//------------------------------------------------------------------------------
JCMp3Parser::~JCMp3Parser()
{
    mpg123_exit();
}
//------------------------------------------------------------------------------
JCMp3Parser* JCMp3Parser::GetInstance( void )
{
	if( ms_pMp3Parser == NULL )
	{
		ms_pMp3Parser = new JCMp3Parser();
	}
	return ms_pMp3Parser;
}
//-----------------------------------------------------------------------------
void JCMp3Parser::DelInstance( void )
{

}
//-----------------------------------------------------------------------------
uint32_t JCMp3Parser::read(uint32_t framesToRead, char* pcmBuf)
{
    size_t bytesToRead = framesToRead * m_bytesPerFrame;
    size_t bytesRead = 0;
    int err = mpg123_read(m_mpg123handle, (unsigned char*)pcmBuf, bytesToRead, &bytesRead);
    if (err == MPG123_ERR)
    {
        LOGE("mpg123_read error %s", mpg123_strerror(m_mpg123handle));
        return 0;
    }
    return static_cast<uint32_t>(bytesRead / m_bytesPerFrame);
}

//------------------------------------------------------------------------------
JCWaveInfo* JCMp3Parser::GetWaveInfo(const char* path)
{
    int error = MPG123_OK;
    m_mpg123handle = mpg123_new(nullptr, &error);
    if (nullptr == m_mpg123handle)
    {
        LOGE("mpg123_new error %s", mpg123_plain_strerror(error));
        return nullptr;
    }
    
    if (mpg123_open(m_mpg123handle, path) != MPG123_OK )
    {
        LOGE("mpg123_open error %s", mpg123_strerror(m_mpg123handle));
        mpg123_close(m_mpg123handle);
        mpg123_delete(m_mpg123handle);
        m_mpg123handle = nullptr;
        return nullptr;
    }

    long rate = 0;
    int mp3Encoding = 0;
    int channel = 0;
    if (mpg123_getformat(m_mpg123handle, &rate, &channel, &mp3Encoding) != MPG123_OK)
    {
        LOGE("mpg123_getformat error %s", mpg123_strerror(m_mpg123handle));
        mpg123_close(m_mpg123handle);
        mpg123_delete(m_mpg123handle);
        m_mpg123handle = nullptr;
        return nullptr;
    }

    JCWaveInfo* pWaveInfo = new JCWaveInfo();
    pWaveInfo->m_kFmtBlock.wavFormat.dwSamplesPerSec = rate;
    pWaveInfo->m_kFmtBlock.wavFormat.wChannels = channel;

    if (mp3Encoding == MPG123_ENC_SIGNED_16)
    {
        m_bytesPerFrame = 2 * channel;
        pWaveInfo->m_kFmtBlock.wavFormat.wBitsPerSample = 16;
    }
    else if (mp3Encoding == MPG123_ENC_FLOAT_32)
    {
        m_bytesPerFrame = 4 * channel;
        pWaveInfo->m_kFmtBlock.wavFormat.wBitsPerSample = 32;
    }
    else
    {
        LOGE("invalid mp3Encoding %d",mp3Encoding);
        mpg123_close(m_mpg123handle);
        mpg123_delete(m_mpg123handle);
        m_mpg123handle = nullptr;
        return nullptr;
    }
    
    /* Ensure that this output format will not change (it could, when we allow it). */
    mpg123_format_none(m_mpg123handle);
    mpg123_format(m_mpg123handle, rate, channel, mp3Encoding);
    /* Ensure that we can get accurate length by call mpg123_length */
    mpg123_scan(m_mpg123handle);
    int totalFrames = mpg123_length(m_mpg123handle);
    
    pWaveInfo->m_fDuration = ((float)totalFrames)/rate;
    pWaveInfo->m_nRealDataSize = totalFrames * m_bytesPerFrame;
    pWaveInfo->m_pData = new unsigned char[pWaveInfo->m_nRealDataSize];
    
    uint32_t framesRead = 0;
    uint32_t framesReadOnce = 0;
    do
    {
        framesReadOnce = read(totalFrames - framesRead,  (char*)pWaveInfo->m_pData + framesRead * m_bytesPerFrame);
        framesRead += framesReadOnce;
    }
    while (framesReadOnce != 0 && framesRead < totalFrames);

    if( framesRead != totalFrames )
    {
        LOGE("JCMp3Parser error 3");
        mpg123_close(m_mpg123handle);
        mpg123_delete(m_mpg123handle);
        m_mpg123handle = nullptr;
        delete pWaveInfo;
        return NULL;
    }
	return pWaveInfo;
}
//------------------------------------------------------------------------------
}
//-----------------------------END FILE--------------------------------

/**
@file			JCMp3Parser.h
@brief			
@author			James
@version		1.0
@date			2014_11_26
*/

#ifndef __JCMp3Parser_H__
#define __JCMp3Parser_H__

#include "JCWaveInfo.h"
#include "mpg123.h"

/** 
 * @brief 读取mp3文件  目前只支持
 * 单声道
 * 16位
 * 采样级别 8000
 * 音频格式 PCM
*/
namespace laya
{
class JCMp3Parser
{
public:

	/** @brief构造函数
	*/
	JCMp3Parser( void );

	/** @brief析构函数
	*/
	~JCMp3Parser( void );

	/*
	 * getInstace()
	*/
	static JCMp3Parser* GetInstance( void );


	static void DelInstance( void );

	/*
	 * 从文件中读取wave数据 
	*/
	JCWaveInfo* GetWaveInfo(const char* path);

    uint32_t read(uint32_t framesToRead, char* pcmBuf);
    
protected:

	static	JCMp3Parser*			ms_pMp3Parser;		//静态的this指针
    
    mpg123_handle*                  m_mpg123handle;
    
    int                             m_bytesPerFrame;

};
}

#endif //__JCMp3Parser_H__

//-----------------------------END FILE--------------------------------

/**
@file			JCEventBase.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

//����ͷ�ļ�
#include <Utils/JCEmitter.h>
//------------------------------------------------------------------------------
namespace laya
{
    JCEventBase::JCEventBase()
    {
		m_nID=0;
		m_pName=0;
		m_nFlags=0; 
		m_nPriority=0;	//���ȼ�
		m_nP1=m_nP2=0;
		//m_pData=0;		//������䣬��Ҫ�Լ��ͷš�
	}
    JCEventBase::~JCEventBase()
    {
        /*
		if( m_pData && (m_nFlags&NEEDDELDATA)){
			delete m_pData;
			m_pData = 0;
		}
        */
	}
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------

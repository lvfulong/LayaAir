/**
@file			JCEventBase.h
@brief			
@author			hugao
@version		1.0
@date			2016_5_11
*/

#ifndef __JCEventBase_H__
#define __JCEventBase_H__

namespace laya
{
	class JCEventBase
    {
	public:
		enum
        {
			HANDLE_ONCE=0x1,//ֻ����һ�Σ������ɷ��������е����������ߡ�����û��Ϊn������
			ISCALLBACK=0x2,	//�����ص�����ѹ����
			UNIQUE=0x4,		//�����ԣ��ڶ�����ֻ��һ����
			NEEDDELDATA=0x5,	//m_pData��Ҫ�ͷ�
		};

    public:
        JCEventBase();
		~JCEventBase();
        //void setDataAsPtr(void* pData) { m_pData = pData; m_nFlags |= NEEDDELDATA; };
		//void setDataAsInt(int data){ m_pData = (void*)data; m_nFlags&=(~NEEDDELDATA);}
		//void* getData(){return m_pData; }
    public:
        int					m_nID;
        char*				m_pName;
        unsigned int		m_nFlags;
        char				m_nPriority;	//���ȼ�
        int					m_nP1, m_nP2;
	protected:
		//void*				m_pData;		//������䣬��Ҫ�Լ��ͷš�

	};
};
//------------------------------------------------------------------------------


#endif //__JCEventBase_H__

//-----------------------------END FILE--------------------------------
#ifndef __JCResource_H__
#define __JCResource_H__

#include <functional>
#include <vector>
#include <utils/ListNode.h>

namespace laya
{
	class JCResManager;
	class JCResStateDispatcher
    {
	public:
		enum ResState
        {
			getting=0,
			ready=1,
			freed=2,
			error=3
		};
		enum ResError
        {
			noError=0,
			downloadError=1,
			decodeError=2
		};
	public:
		typedef std::function<void(JCResStateDispatcher*)>      ResOnReadyCB;
		typedef std::function<void(JCResStateDispatcher*,int)>  ResOnErrCB;
        virtual~JCResStateDispatcher(){}
        JCResStateDispatcher();

		ResState getState(){return m_nResState;};

		//可能会触发回调。
		void setState( ResState p_state );

		//资源准备好之后的回调。可以调用多次，添加多个回调
		void setOnReadyCB(ResOnReadyCB cb );

		//资源出错了的回调。
		void setOnErrorCB(ResOnErrCB cb);

		void setOnFreeCB(ResOnReadyCB cb );

		virtual bool restoreRes() = 0;

    public:

        //资源准备好了的回调。回调函数在执行之后就会被删除
        std::vector<ResOnReadyCB >	    m_allOnReadyCB,m_tmpOnReadyCB;
        std::vector<ResOnErrCB >	    m_allOnErrCB,m_tmpOnErrCB;
        std::vector<ResOnReadyCB >	    m_allOnFreeCB,m_tmpOnFreeCB;
        ResState		                m_nResState;	    //当前的状态。外部禁止修改
        int				                m_nErrNo;	        //错误代码
	};
}
//------------------------------------------------------------------------------


#endif //__JCResource_H__

//-----------------------------END FILE--------------------------------

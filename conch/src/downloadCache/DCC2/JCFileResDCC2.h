/**
 * 新的DCC管理
 * 可以被接管
 * 
*/

#ifndef __JCFILERESDCC2_H__
#define __JCFILERESDCC2_H__

#include "../../resource/JCFileResManager.h"
#include "DCCInterface.h"

namespace laya{
	class JCFileResDCC2 : public JCFileRes,public IDCCInterface
    {
    public:
        JCFileResDCC2();
        ~JCFileResDCC2();

        /** @brief 加载资源。注意只允许调试或者manager调用。否则可能会同一个资源加载多次。
         *  @param[in] 
         *  @return 
        */
        void load(const char* p_pszURL, JCSharedBuffer* pSyncResult) override;

	};

}

#endif
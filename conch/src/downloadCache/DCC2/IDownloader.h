#ifndef _IDOWNLOADER__H_______
#define _IDOWNLOADER__H_______

#include <functional>

namespace laya{
    class JCBuffer;
    class IDownloader{
    public:
        typedef std::function<void(JCBuffer& p_Buff, const char* localPath)> onDownloadedFunc;
        virtual void download(const char* pszUrl, onDownloadedFunc onok)=0;
    };
}
#endif
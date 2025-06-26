#ifndef _IDOWNLOADER__H_______
#define _IDOWNLOADER__H_______

#include <functional>
#include <utils/Data.h>

namespace laya
{
    class IDownloader
    {
    public:
        typedef std::function<void (const std::shared_ptr<Data>& data, const char* localPath)> onDownloadedFunc;
        virtual void download(const char* pszUrl, onDownloadedFunc onok) = 0;
    };
}
#endif

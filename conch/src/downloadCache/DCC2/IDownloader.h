#ifndef _IDOWNLOADER__H_______
#define _IDOWNLOADER__H_______

namespace laya{
    class IDownloader{
        virtual void download(const char* pszUrl)=0;
    };
}
#endif
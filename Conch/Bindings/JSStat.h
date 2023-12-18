#ifndef __JSStat_H__
#define __JSStat_H__

#include <Binder/JSInterface.h>

namespace laya 
{

    class JSStat
    {
    public:

        static void exportJS(Context& context);

		~JSStat();

		int getSize();

    public:
        bool isDirectory();

		bool isFile();

		bool m_isDirectory = false;

		bool m_isFile = false;

		int m_nSize = 0;
    };

}
#endif
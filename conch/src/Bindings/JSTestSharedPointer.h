#ifndef __JSTestSharedPointer_H__
#define __JSTestSharedPointer_H__


#include <stdio.h>
#include <jsbind/JSBind.h>
#include <functional>

namespace laya 
{
    class JSTestSharedPointer : public std::enable_shared_from_this<JSTestSharedPointer>
    {
    public:
	    static void exportJS(jsbind::Object& context);

	    JSTestSharedPointer();

	    ~JSTestSharedPointer();

    public:
		void test(JSTestSharedPointer* dd);
        void test2(std::shared_ptr<JSTestSharedPointer> dd);
    private:
        std::string m_name = "dddddddddddddddwwwwwwwwwwwww";

    };
}

#endif
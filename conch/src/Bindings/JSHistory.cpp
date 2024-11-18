#include "JSHistory.h"
#include "../../JCConch.h"

namespace laya 
{
    int JSHistory::getLength() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlHistoryLength();
        }
        return 0;
    }
    void JSHistory::back() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlBack();
        }
    }
    void JSHistory::forward() 
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlBack();
        }
    }
    void JSHistory::go(int step) 
    {
        if (JCConch::s_pConch)
        {
            return JCConch::s_pConch->urlGo(step);
        }
    }
    void JSHistory::push(const char* strUrl)
    {
        if (JCConch::s_pConch) 
        {
            return JCConch::s_pConch->urlHistoryPush(strUrl);
        }
    }
    void JSHistory::exportJS(jsbind::Object& context)
    {
        jsbind::global_class_<JSHistory> class_binding;
		class_binding.class_function("back", &JSHistory::back);
		class_binding.class_function("forward", &JSHistory::forward);
		class_binding.class_function("go", &JSHistory::go);
		class_binding.class_function("_push", &JSHistory::push);
         //class_property必须在下面，否则导不出class_function
        class_binding.class_property("length", &JSHistory::getLength);
        context.global_class_("history", class_binding);
    }
}

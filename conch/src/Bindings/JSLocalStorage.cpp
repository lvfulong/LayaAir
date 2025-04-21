#include "JSLocalStorage.h"
#include "JCConch.h"
#include <utils/JCMemorySurvey.h>
#include <utils/Log.h>
#include <webstorage/WebStorage.h>

namespace laya
{
    jsvm_value JSLocalStorage::getItem(const std::string &key)
    {
        auto item = JCConch::s_pConch->getLocalStorage().getItem(key);  
        if (item)
        {
            return jsbind::Make<std::string>(item.value());  
        }
        return jsbind::MakeNull();
    }
    void JSLocalStorage::setItem(const std::string &key, const std::string &value)
    {
        JCConch::s_pConch->getLocalStorage().setItem(key, value);
    }
    void JSLocalStorage::removeItem(const std::string &key)
    {
        JCConch::s_pConch->getLocalStorage().removeItem(key);
    }
    int JSLocalStorage::getLength()
    {
        return JCConch::s_pConch->getLocalStorage().length();
    }
    void JSLocalStorage::clear()
    {
        JCConch::s_pConch->getLocalStorage().clear();
    }
    jsvm_value JSLocalStorage::key(int index)
    {
        auto key = JCConch::s_pConch->getLocalStorage().key(index);         
        if (key)
        {
            return jsbind::Make<std::string>(key.value());
        }
        return jsbind::MakeNull();
    }
    void JSLocalStorage::exportJS(jsbind::Object &context)
    {
        jsbind::global_class_<JSLocalStorage> class_binding;
        class_binding.class_function("getItem", &JSLocalStorage::getItem);
        class_binding.class_function("setItem", &JSLocalStorage::setItem);
        class_binding.class_function("removeItem", &JSLocalStorage::removeItem);
        class_binding.class_function("clear", &JSLocalStorage::clear);
        class_binding.class_function("key", &JSLocalStorage::key);
        //class_property必须在下面，否则导不出class_function 
        class_binding.class_property("length", &JSLocalStorage::getLength);
        context.global_class_("localStorage", class_binding);//window.localStorage
    }
} // namespace laya

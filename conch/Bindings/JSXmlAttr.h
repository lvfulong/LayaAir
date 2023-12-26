#ifndef _JSXmlAttr_H  
#define _JSXmlAttr_H  

#include <binder/JSInterface.h>
#include <rapidxml/rapidxml.hpp>

namespace laya
{
    class  JSXmlAttr
    {
    public:
        static void exportJS(Context& context);

        JSXmlAttr();
        ~JSXmlAttr();
        const char *getNodeName();
        const char *getNodeValue();
        const char *getTextContent();

    public:
        std::string  m_nodeName;
        std::string  m_nodeValue;
    };
}
#endif 
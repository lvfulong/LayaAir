#include "JSXmlAttr.h"
namespace laya
{
    JSXmlAttr::JSXmlAttr()
    {
        AdjustAmountOfExternalAllocatedMemory(64000);
        JCMemorySurvey::GetInstance()->newClass("XmlAttr", 64000, this);
    }
    JSXmlAttr::~JSXmlAttr()
    {
        JCMemorySurvey::GetInstance()->releaseClass("XmlAttr", this);
    }
    const char * JSXmlAttr::getNodeName()
    {
        return m_nodeName.c_str();
    }
    const char * JSXmlAttr::getNodeValue()
    {
        return m_nodeValue.c_str();
    }
    const char *JSXmlAttr::getTextContent()
    {
        return m_nodeValue.c_str();
    }
    void JSXmlAttr::exportJS(Context& context)
    {
        class_<JSXmlAttr> class_binding;
        class_binding.constructor<>();
        class_binding.property("nodeValue", &JSXmlAttr::getNodeValue);
        class_binding.property("nodeName", &JSXmlAttr::getNodeName);
        class_binding.property("textContent", &JSXmlAttr::getTextContent);
        context.class_("_XmlAttr", class_binding);
    }
}

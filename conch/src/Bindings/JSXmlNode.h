#ifndef _JSXmlNode_H  
#define _JSXmlNode_H  
#include <vector>
#include "JSXmlAttr.h"
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml_print.hpp>

namespace laya
{
    class  JSXmlNode
    {
    public:
        static void exportJS(jsbind::Object& context);
        JSXmlNode();
        virtual ~JSXmlNode();
        bool  hasChildNodes();
        JSXmlNode* insertBefore(JSXmlNode* newChild, JSXmlNode* refChild);
        JSXmlNode* replaceChild(JSXmlNode* newChild, JSXmlNode* oldChild);
        JSXmlNode* removeChild(JSXmlNode* oldChild);
        JSXmlNode* appendChild(JSXmlNode* newChild);
        jsvm::Value  getParentNode();
        jsvm::Value  getFirstChild();
        jsvm::Value  getLastChild();
        jsvm::Value  getPreviousSibling();
        jsvm::Value  getNextSibling();
        const char *getNodeName();
        const char *getNodeValue();
        const char *getTextContent();
        virtual jsvm::Value  getChildNodes();
        virtual jsvm::Value  getAttributes();
        void initXmlNode(rapidxml::xml_node<>* node,bool isRoot);
    public:
        std::vector<JSXmlNode*> m_childNodes;
        std::vector<JSXmlAttr*> m_attributes;
        enum { allchildsid, allattribid };
        jsbind::Persistent m_jsChildNodes; bool m_bCreateChilds;
        jsbind::Persistent m_jsAttribs; bool m_bCreateAttribs;
        JSXmlNode* m_parentNode;
        std::string  m_nodeName;
        std::string  m_nodeValue;
    };
    class JSXmlDocument :public JSXmlNode
    {
    public:
        static void exportJS(jsbind::Object& context);
        JSXmlDocument();
        ~JSXmlDocument();
        void parse(const char* str);
        jsvm::Value  getChildNodes();
    public:
        class rapidxml::xml_document<>*m_document;
    };
}
#endif 
#include "JSXmlNode.h"
#include <sstream>
#include <utils/Log.h>

namespace laya
{
    JSXmlNode::JSXmlNode()
    {
        m_parentNode = NULL;
        AdjustAmountOfExternalAllocatedMemory(64000);
        m_bCreateChilds = false;
        m_bCreateAttribs = false;
        JCMemorySurvey::GetInstance()->newClass("XmlNode", 64000, this);
    }
    JSXmlNode::~JSXmlNode()
    {
        m_parentNode = NULL;
        m_childNodes.clear();
        m_attributes.clear();
        JCMemorySurvey::GetInstance()->releaseClass("XmlNode", this);
    }
    bool  JSXmlNode::hasChildNodes()
    {
        return true;
    }
    JSXmlNode* JSXmlNode::insertBefore(JSXmlNode* newChild, JSXmlNode* refChild)
    {
        return this;
    }
    JSXmlNode* JSXmlNode::replaceChild(JSXmlNode* newChild, JSXmlNode* oldChild)
    {
        return this;
    }
    JSXmlNode* JSXmlNode::removeChild(JSXmlNode* oldChild)
    {
        return this;
    }
    JSXmlNode* JSXmlNode::appendChild(JSXmlNode* newChild)
    {
        return this;
    }
    JsValue  JSXmlNode::getParentNode()
    {
         return jsbind::Local::Make<JSXmlNode*>(m_parentNode);
    }
    JsValue  JSXmlNode::getFirstChild()
    {
        return jsbind::Local::MakeNull();
    }
    JsValue JSXmlNode::getLastChild()
    {
        return jsbind::Local::MakeNull();
    }
    JsValue  JSXmlNode::getPreviousSibling()
    {
        return jsbind::Local::MakeNull();
    }
    JsValue  JSXmlNode::getNextSibling()
    {
        return jsbind::Local::MakeNull();
    }
    const char * JSXmlNode::getNodeName()
    {
        if (m_nodeName.size() == 0)
        {
            return "#cdata-section";
        }
        return m_nodeName.c_str();
    }
    const char * JSXmlNode::getNodeValue()
    {
        return m_nodeValue.c_str();
    }
    const char * JSXmlNode::getTextContent()
    {
        return m_nodeValue.c_str();
    }
    JsValue JSXmlNode::getChildNodes()
    {
        if (!m_bCreateChilds)
        {
            JsValue pJSValue = jsbind::Local::Make<std::vector<JSXmlNode*>>(m_childNodes);
            m_jsChildNodes = jsbind::Persistent(pJSValue);
            m_bCreateChilds = true;
        }
        return m_jsChildNodes.getHandle();
    }
    JsValue JSXmlNode::getAttributes()
    {
        if (!m_bCreateAttribs)
        {
            m_bCreateAttribs = true;
            JsValue pJSValue = jsbind::Local::Make<std::vector<JSXmlAttr*>>(m_attributes);
            m_jsAttribs = jsbind::Persistent(pJSValue);
        }
        return m_jsAttribs.getHandle();
    }
	void JSXmlNode::initXmlNode(rapidxml::xml_node<>* node,bool isRoot)
	{
		m_nodeName = ""; m_nodeName+= (node->name() == NULL ? "" : node->name());
		m_nodeValue = "";m_nodeValue+=(node->value() == NULL ? "" : node->value());
		int childsize=rapidxml::count_children(node);
		int i=0;
        if(isRoot)
        {
            if(childsize>1)
            childsize=1;
        }
		m_childNodes.resize(childsize);
		class rapidxml::xml_node<>* cnode = node->first_node();
		for(i=0; i<childsize; i++)
		{
			JSXmlNode* childNode = new JSXmlNode();
			childNode->initXmlNode(cnode,false);
			childNode->m_parentNode=this;
			m_childNodes[i]=childNode;
			cnode = cnode->next_sibling();
		}
		class rapidxml::xml_attribute<>* attr = node->first_attribute();
		int attrsize=rapidxml::count_attributes(node);
		m_attributes.resize(attrsize);
		for(i=0;i<attrsize;i++)
		{
			JSXmlAttr* attribute = new JSXmlAttr();
			attribute->m_nodeName = "";attribute->m_nodeName+= (attr->name() == NULL ? "" : attr->name());
			attribute->m_nodeValue = "";attribute->m_nodeValue+=(attr->value() == NULL ? "" : attr->value());

			m_attributes[i]=attribute;
			attr = attr->next_attribute();
		}
	}
    JSXmlDocument::JSXmlDocument()
    {
        m_document = new rapidxml::xml_document<>();
    }
    JSXmlDocument::~JSXmlDocument()
    {
        if (m_document != NULL)
        {
            delete m_document;
            m_document = NULL;
        }
    }
    void JSXmlDocument::parse(const char* str)
    {
        std::stringstream stream(str);
        rapidxml::file<>* fdoc=NULL;
        //try
        //{
            fdoc = new rapidxml::file<>(stream);
            m_document->parse<0>(fdoc->data());
        //}
        //catch (...)
        //{
        //    LOGE("JSXmlDocument::parse error:%s", str);
        //}
        initXmlNode(m_document,true);
        if(fdoc!=NULL)delete fdoc;
    }
    JsValue  JSXmlDocument::getChildNodes()
    {
        return JSXmlNode::getChildNodes();
    }
    void JSXmlNode::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSXmlNode> class_binding;
        class_binding.constructor<>();
        class_binding.property("nodeValue", &JSXmlNode::getNodeValue);
        class_binding.property("nodeName", &JSXmlNode::getNodeName);
        class_binding.property("textContent", &JSXmlNode::getTextContent);
        class_binding.property("childNodes", &JSXmlNode::getChildNodes);
        class_binding.property("attributes", &JSXmlNode::getAttributes);
        context.class_("_XmlNode", class_binding);
    }
    void JSXmlDocument::exportJS(jsbind::Object& context)
    {
        jsbind::class_<JSXmlDocument> class_binding;
        class_binding.constructor<>();
        class_binding.property("childNodes", &JSXmlDocument::getChildNodes);
        context.class_("_XmlDocument", class_binding);
    }
}

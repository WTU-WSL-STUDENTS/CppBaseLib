/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-18 20:19:50
 * @LastEditors: like
 * @LastEditTime: 2021-08-20 18:22:08
 */
#ifndef SYSTEM_XML_HPP
#define SYSTEM_XML_HPP
/* Common/thirdparty/tinyxml-2-6-2/docs/index.html
 * https://www.w3school.com.cn/xml/xml_encoding.asp
*/
#include <System.List.hpp>

namespace System::Xml
{
    typedef enum _XmlNodeType/* https://docs.microsoft.com/zh-cn/dotnet/api/system.xml.xmlnodetype?view=net-5.0*/
    {
        None                    = 0, 	
        Element                 = 1, 	//元素，        <item>
        Attribute               = 2,    //属性，        id='123'
        Text                    = 3, 	//文本          Hello       节点的文本内容。Text 节点不能具有任何子节点。 它可以显示为 Attribute、DocumentFragment、Element 和 EntityReference 节点的子节点。
        CDATA                   = 4,    //CDATA         <![CDATA[my escaped text]]>
        EntityReference         = 5,    //实体引用      &num; 
        Entity                  = 6, 	//实体声明      <!ENTITY...>  
        ProcessingInstruction   = 7, 	//处理指令      <?pi test?>   
        Comment                 = 8, 	//注释          <!-- my comment -->
        Document                = 9, 	//作为文档树的根的文档对象提供对整个 XML 文档的访问。
        DocumentType            = 10, 	//文档类型声明  <!DOCTYPE...>
        DocumentFragment        = 11,   //文档片段。
        Notation                = 12, 	//文档类型声明中的表示法    <!NOTATION...>
        Whitespace 	            = 13, 	//标记间的空白。
        SignificantWhitespace 	= 14, 	//混合内容模型中标记间的空格或范围内的空格。    xml:space="preserve"
        EndElement              = 15, 	//末尾元素标记  </item>
        EndEntity               = 16, 	//由于调用 ResolveEntity() 而使 XmlReader 到达实体替换的末尾时返回。
        XmlDeclaration 	        = 17    //XML声明       <?xml version='1.0'?>   XmlDeclaration 节点必须是文档中的第一个节点。 它不能有子节点。 它是 Document 节点的子节点。 它可以具有提供版本和编码信息的特性。
    }XmlNodeType;
    
    class XmlNode;
    class XmlAttribute;
    typedef List<XmlAttribute*>  XmlAttributeCollection;
    typedef List<XmlNode*>       XmlNodeList;
    class XmlDeclaration ;
    class XmlDocument;
    // typedef struct _XmlHead
    // {
    //     #define XML_HEAD_TYPE_1_0 "1.0"
    //     #define XML_HEAD_ENCODING 
    //     const char* type;
    //     const char* encoding;
    //     bool standalone;
    // }XmlHead;
    // class IXmlNode
    // {
    // public:
    //     virtual bool LoadXmlNode() = 0;
    //     virtual void ToXmlString(char** buffer, size_t* len) = 0;
    // };
};
class System::Xml::XmlNode
{
protected:
    XmlNode* parent;
    XmlNode* pre;
    XmlNode* next;
    XmlAttributeCollection* attributes;
    XmlNodeList* childNodeList;
    XmlNodeType nodeType;
    virtual XmlNode* OwnerDocument(XmlNode* node)
    {
        return node->parent ?  OwnerDocument(node->parent) : node;
    }
    XmlNode(XmlNode* _parent, XmlNode* _pre, XmlNode* _next, XmlNodeType type) : 
        parent(_parent), pre(_pre), next(_next), attributes(NULL), childNodeList(NULL), nodeType(type){}
    XmlNode(const XmlNode* copy) : 
        parent(copy->parent), pre(copy->pre), next(copy->next), attributes(copy->attributes->Clone()), childNodeList(copy->childNodeList->Clone()), nodeType(copy->nodeType){}
public:
    ~XmlNode()
    {
        for(size_t i = 0; i < attributes->Count(); i++)
        {
            delete (*attributes)[i];
            (*attributes)[i] = NULL;
        }
        delete attributes;
        attributes = NULL;
        for(size_t i = 0; i < childNodeList->Count(); i++)
        {
            delete (*childNodeList)[i];
            (*childNodeList)[i] = NULL;
        }
        delete childNodeList;
        childNodeList = NULL;
    }
    virtual bool HasChildNodes()
    {
        return childNodeList.Count();
    }
    virtual XmlNode* FirstChild()
    {
        return HasChildNodes() ? childNodeList[0] : NULL;
    }
    virtual XmlNode* LastChild()
    {
        return HasChildNodes() ? childNodeList[childNodeList.Count() - 1] : NULL;
    }
    virtual XmlNodeList* ChildNodes()
    {
        return &childNodeList;
    }
    
    virtual XmlAttributeCollection* Attributes()
    {
        return &attributes;
    }
    virtual XmlNodeType NodeType()
    {
        return nodeType;
    }
    virtual XmlDocument* OwnerDocument()
    {
        return (XmlDocument*)OwnerDocument(this);
    }
    XmlNode* Parent()
    {
        return parent;
    }
    XmlNode* PreviousNode()
    {
        return pre;
    }
    XmlNode* NextNode()
    {
        return next;
    }
    void AppendChild(XmlNode** newNode)
    {
        (*newNode)->pre = this;
        (*newNode)->next = next;
        next->pre = *newNode;
        next = *newNode;
    }
    XmlNode* Clone()
    {
        return new XmlNode(this);
    }
};

class System::Xml::XmlDeclaration  : public System::Xml::XmlNode
{
private:
    const char* str;
public:
    const char* Encoding;
    const char* Version;
    const char* Standalone;
    XmlDeclaration () : XmlNode(NULL, NULL, NULL, XmlNodeType::XmlDeclaration)
    {
        
    }
};
#endif
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
        Element                 = 1, 	//Ԫ�أ�        <item>
        Attribute               = 2,    //���ԣ�        id='123'
        Text                    = 3, 	//�ı�          Hello       �ڵ���ı����ݡ�Text �ڵ㲻�ܾ����κ��ӽڵ㡣 ��������ʾΪ Attribute��DocumentFragment��Element �� EntityReference �ڵ���ӽڵ㡣
        CDATA                   = 4,    //CDATA         <![CDATA[my escaped text]]>
        EntityReference         = 5,    //ʵ������      &num; 
        Entity                  = 6, 	//ʵ������      <!ENTITY...>  
        ProcessingInstruction   = 7, 	//����ָ��      <?pi test?>   
        Comment                 = 8, 	//ע��          <!-- my comment -->
        Document                = 9, 	//��Ϊ�ĵ����ĸ����ĵ������ṩ������ XML �ĵ��ķ��ʡ�
        DocumentType            = 10, 	//�ĵ���������  <!DOCTYPE...>
        DocumentFragment        = 11,   //�ĵ�Ƭ�Ρ�
        Notation                = 12, 	//�ĵ����������еı�ʾ��    <!NOTATION...>
        Whitespace 	            = 13, 	//��Ǽ�Ŀհס�
        SignificantWhitespace 	= 14, 	//�������ģ���б�Ǽ�Ŀո��Χ�ڵĿո�    xml:space="preserve"
        EndElement              = 15, 	//ĩβԪ�ر��  </item>
        EndEntity               = 16, 	//���ڵ��� ResolveEntity() ��ʹ XmlReader ����ʵ���滻��ĩβʱ���ء�
        XmlDeclaration 	        = 17    //XML����       <?xml version='1.0'?>   XmlDeclaration �ڵ�������ĵ��еĵ�һ���ڵ㡣 ���������ӽڵ㡣 ���� Document �ڵ���ӽڵ㡣 �����Ծ����ṩ�汾�ͱ�����Ϣ�����ԡ�
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
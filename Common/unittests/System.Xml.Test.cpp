/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-19 17:19:36
 * @LastEditors: like
 * @LastEditTime: 2021-08-20 10:28:56
 */
#define TINY_XML
#ifdef TINY_XML
#include <tinyxml.h>
typedef TiXmlDocument       XmlDocument;    /* xml文档  */
typedef TiXmlComment        XmlComment;     /* xml注释  */
typedef TiXmlDeclaration    XmlHead;        /* xml头    */
typedef TiXmlElement        XmlElement;     /* xml节点  */
typedef TiXmlAttribute      XmlAttribute;   /* xml属性  */
typedef TiXmlText           XmlText;        /* xml文本  */
#else
#include <System.Xml.hpp>
using namespace System.Xml;
#endif

const char* xmlFilePath = "madeByHand2.xml";
int main()
{
    XmlDocument doc( "demo.xml" );
    if(!doc.LoadFile())
    {
        XmlHead * decl = new XmlHead( "1.0", "abc", "23" );
        doc.LinkEndChild( decl );        
    }
	XmlElement * element = new XmlElement( "Hello" );
	doc.LinkEndChild( element );
	
	XmlText * text = new XmlText( "World" );
	element->LinkEndChild( text );
	
	doc.SaveFile(xmlFilePath);
}
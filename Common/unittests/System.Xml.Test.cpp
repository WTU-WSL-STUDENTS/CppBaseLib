/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-19 17:19:36
 * @LastEditors: like
 * @LastEditTime: 2021-09-13 10:59:04
 */
#include <System.Xml.hpp>
using namespace System::Xml;

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
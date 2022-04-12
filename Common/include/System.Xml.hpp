/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-18 20:19:50
 * @LastEditors: like
 * @LastEditTime: 2022-02-08 15:51:52
 */
#ifndef SYSTEM_XML_HPP
#define SYSTEM_XML_HPP
/* Common/thirdparty/tinyxml-2-6-2/docs/index.html
 * https://www.w3school.com.cn/xml/xml_encoding.asp
*/
#include <tinyxml.h>
// #include <System.List.hpp>

namespace System::Xml
{
    typedef TiXmlDocument       XmlDocument;    /* xml�ĵ�  */
    typedef TiXmlComment        XmlComment;     /* xmlע��  */
    typedef TiXmlDeclaration    XmlHead;        /* xmlͷ    */
    typedef TiXmlElement        XmlElement;     /* xml�ڵ�  */
    typedef TiXmlAttribute      XmlAttribute;   /* xml����  */
    typedef TiXmlText           XmlText;        /* xml�ı�  */
}
#endif
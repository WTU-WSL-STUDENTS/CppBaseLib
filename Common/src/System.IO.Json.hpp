/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 16:07:30
 * @LastEditors: like
 * @LastEditTime: 2021-08-12 11:19:24
 */
// /*
//  * @Description: 
//  * @Version: 1.0
//  * @Autor: like
//  * @Date: 2021-08-11 16:07:30
//  * @LastEditors: like
//  * @LastEditTime: 2021-08-11 18:57:31
//  */
// #ifndef SYSTEM_IO_JSON_HPP
// #define SYSTEM_IO_JSON_HPP

// #include <System.Convert.hpp>
// #include <System.Tree.hpp>
// #include <System.IO.Ini.hpp>
// #include <stdio.h>

// #define JSON_MAX_KEY_SIZE 255
// #define JSON_MAX_VAL_SIZE 255
// namespace System::IO
// {
//     class JsNode;
//     class JsNodeList;
// }
// class System::IO::JsNode:INode<char*>
// {
// private:
//     char key[JSON_MAX_KEY_SIZE];
// public:
//     JsNode() : key({0}), val(new char[JSON_MAX_VAL_SIZE]{0}){}
//     static JsNode* ConvertFromFileLine(const char* str)
//     {
//         const char* split = strchr(str, ':');
//         if(NULL == split)
//         {
//             return NULL;
//         }
//         JsNode* node = new JsNode();
//         memcpy(node->key, str, splite - str);
//         memcpy(node->val, split + 1, strlen(str) - splite - 2/* "...\n\0" */);
//         return node;
//     }
// };
// class System::IO::JsNodeList : std::vector<JsNode>
// {
    
// }

// #endif
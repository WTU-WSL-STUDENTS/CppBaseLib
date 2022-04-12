/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-06 15:51:07
 * @LastEditors: like
 * @LastEditTime: 2022-01-07 10:21:33
 */
#ifndef IPLUGIN_CONTROLLER_HPP
#define IPLUGIN_CONTROLLER_HPP

#include "IPluginDef.h"
#include <string>
#include <vector>
#include <System.IO.FileStream.hpp>

using namespace std;

class PluginController
{
public:
    PluginController(string strPluginWorkspace) : m_strPluginWorkspace(strPluginWorkspace)
    {
        System::IO::File::
    }
private:
    string m_strPluginWorkspace;
    vector<string> vecPluginNames;
};

#endif
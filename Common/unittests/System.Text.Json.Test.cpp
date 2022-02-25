/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-06 11:01:40
 * @LastEditors: like
 * @LastEditTime: 2022-02-21 19:42:28
 */
#include <System.Text.Json.hpp>
#include <System.IO.File.hpp>
#include <fstream>
#include <iostream>

using namespace std;
using namespace System::IO;

const char* jsonPath = "test.json";

namespace ns
{
    typedef struct _Object
    {
        std::string currency;
        double value;
    }Object;
    inline void to_json(json& j, const Object& p) 
    {
        j = json{"object",{"currency", p.currency}, {"value", p.value}};
    }
    inline void from_json(const json& j, Object& p) 
    {
        j["object"].at("currency").get_to(p.currency);
        j["object"].at("value").get_to(p.value);
    }
    //NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE
    struct Info
    {
    public:
        string name;
        double pi;
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Info, name, pi);
    };
}

void CreateJson()
{
    json j;
    double pi = 3.141;
    j["pi"] = pi;
    j["happy"] = true;
    j["name"] = "Niels";
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    j["list"] = { 1, 0, 2 };
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };

    json j2 = 
    {
        {"pi", pi},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", 
            {
                {"everything", 42}
            }
        },
        {"list", {1, 0, 2}},
        {"object", 
            {
                {"currency", "USD"},
                {"value", 42.99}
            }
        }
    };
    printf("j1\n%s\n", j.dump(4).c_str());
    printf("j2\n%s\n", j2.dump(2).c_str());
    string str = j.dump(4);
    FileStream* fs = File::Create(jsonPath);
    fs->Write(str.c_str(), str.length());
    delete fs;
}

json ReadJson()
{
    printf("Begin ReadJson\n");
    json j;
    size_t byteCount = File::FileSize(jsonPath);
    char* str = (char*)malloc(byteCount);
    size_t readedCount = File::ReadAllBytes(jsonPath, str, byteCount);
    if(readedCount != byteCount)
    {
        printf("File Size : %d , Readed Size : %d, %s", byteCount, readedCount, str);
    }
    j = json::parse(str);
    free(str);
    return j;
}

int main()
{
    if(!File::Exists(jsonPath))
    {
        CreateJson();
        return 0;
    }
    json j = ReadJson();
    printf("Readed Json : \n%s\n", j.dump(4).c_str());

    string name = j["name"].get<string>();
    double pi = j["pi"].get<double>();
    printf("%s, %f\n", name.c_str(), pi);

    auto obj  = j.get<ns::Object>();
    auto info = j.get<ns::Info>();
    printf("get<Object> : %10s,%10f\n", obj.currency.c_str(), obj.value);
    printf("get<Info>   : %10s,%10f\n", info.name.c_str(), info.pi);
    return 0;
}
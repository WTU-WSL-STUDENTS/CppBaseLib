/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-06 11:01:40
 * @LastEditors: like
 * @LastEditTime: 2021-09-14 17:35:01
 */
#include <System.Text.Json.hpp>


int main()
{
    // create an empty structure (null)
    json j;
    double pi = 3.141;
    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = pi;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };

    // instead, you could also write (which looks very similar to the JSON above)
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
    return 0;
}
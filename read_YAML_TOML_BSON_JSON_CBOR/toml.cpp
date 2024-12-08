//    ```toml
//    name = "MyApplication"
//    version = 1

//    [features]
//    awesome = true
//    ```

#include <iostream>
#include "cpptoml.h"

int main() {
    auto config = cpptoml::parse_file("config.toml");

    auto name = config->get_as<std::string>("name");
    auto version = config->get_as<int>("version");
    auto awesome = config->get_table("features")->get_as<bool>("awesome");

    std::cout << "Name: " << *name << "\nVersion: " << *version 
                << "\nAwesome: " << std::boolalpha << *awesome << std::endl;

    return 0;
}
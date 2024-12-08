// ```yaml
// name: MyApplication
// version: 1
// features:
//    awesome: true
// ```

#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
      YAML::Node config = YAML::LoadFile("config.yaml");

      std::string name = config["name"].as<std::string>();
      int version = config["version"].as<int>();
      bool awesome = config["features"]["awesome"].as<bool>();

      std::cout << "Name: " << name << "\nVersion: " << version 
               << "\nAwesome: " << std::boolalpha << awesome << std::endl;

      return 0;
}

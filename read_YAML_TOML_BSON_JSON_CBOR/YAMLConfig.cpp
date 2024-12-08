#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

class YAMLConfig {
public:
    YAMLConfig(const std::string& config_file);
    void loadConfig();

    std::string getName() const { return name_; }
    int getVersion() const { return version_; }
    bool isAwesome() const { return awesome_; }

private:
    std::string config_file_;
    std::string name_;
    int version_;
    bool awesome_;
};

YAMLConfig::YAMLConfig(const std::string& config_file)
    : config_file_(config_file), version_(0), awesome_(false) {}

void YAMLConfig::loadConfig() {
    YAML::Node config = YAML::LoadFile(config_file_);

    if (config["name"]) {
        name_ = config["name"].as<std::string>();
    }

    if (config["version"]) {
        version_ = config["version"].as<int>();
    }

    if (config["features"] && config["features"]["awesome"]) {
        awesome_ = config["features"]["awesome"].as<bool>();
    }
}

int main() {
    // Example usage
    YAMLConfig yaml_config("config.yaml");

    yaml_config.loadConfig();

    std::cout << "Name: " << yaml_config.getName() 
              << "\nVersion: " << yaml_config.getVersion() 
              << "\nAwesome: " << std::boolalpha << yaml_config.isAwesome() 
              << std::endl;

    return 0;
}
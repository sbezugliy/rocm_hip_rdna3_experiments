#include "AppConfig.h"
#include <yaml-cpp/yaml.h>

AppConfig::AppConfig(const std::string& config_file)
    : config_file_(config_file), version_(0), awesome_(false) {}

void AppConfig::loadConfig() {
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

std::string AppConfig::getName() const {
    return name_;
}

int AppConfig::getVersion() const {
    return version_;
}

bool AppConfig::isAwesome() const {
    return awesome_;
}
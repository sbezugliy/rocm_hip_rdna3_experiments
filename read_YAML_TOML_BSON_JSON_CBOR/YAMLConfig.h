#ifndef YAML_CONFIG_H
#define YAML_CONFIG_H

#include <string>

class YAMLConfig {
public:
    // Constructor that initializes with a configuration file path
    YAMLConfig(const std::string& config_file);

    // Loads configuration from the YAML file
    void loadConfig();

    // Accessor methods for configuration values
    std::string getName() const;
    int getVersion() const;
    bool isAwesome() const;

private:
    std::string config_file_;  // The path to the YAML configuration file
    std::string name_;         // Name of the application
    int version_;              // Application version
    bool awesome_;             // Feature flag indicating if awesome feature is enabled
};

#endif // YAML_CONFIG_H
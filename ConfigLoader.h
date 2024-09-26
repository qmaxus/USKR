#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

class ConfigLoader {
public:
    static std::string loadConfig(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open config file.");
        }

        nlohmann::json config;
        file >> config;

        return config.dump(); // Возвращаем строку JSON
    }
};

#endif // CONFIG_LOADER_H
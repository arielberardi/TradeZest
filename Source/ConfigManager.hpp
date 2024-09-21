#pragma once

#include <string>
#include <unordered_map>

class ConfigManager
{
  public:
    static ConfigManager& Instance()
    {
        static ConfigManager instance;
        return instance;
    };

    bool Load(const std::string& filePath);
    std::string Get(const std::string& key, const std::string& defaultValue) const;

  private:
    ConfigManager(){};

    void parseLine(std::string line);

    std::unordered_map<std::string, std::string> m_Config{};
};

// Need to understand why m_Config is empty after the "Load" thing

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include <boost/algorithm/string.hpp>

#include "ConfigManager.hpp"

static constexpr char CONFIG_DELIMITER = '=';

void ConfigManager::parseLine(std::string line)
{
    // We look for comments
    // If the comment exist, we remove it from the line
    // If the line was just a command, would be empty and would be ignore
    size_t pos = line.find('#');
    if (pos != std::string::npos)
    {
        line = line.substr(0, pos);
    }

    if (line.empty())
    {
        return;
    }

    size_t delimiterPos = line.find(CONFIG_DELIMITER);
    if (delimiterPos == std::string::npos)
    {
        std::cerr << "Config: invalid config input" << std::endl;
        return;
    }

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 1);

    boost::trim(key);
    boost::trim(value);

    m_Config[key] = value;
}

bool ConfigManager::Load(const std::string& filePath)
{
    // File must exists
    if (!std::filesystem::exists(filePath))
    {
        std::cerr << "Config file does not exist: " << filePath << std::endl;
        return false;
    }

    std::fstream file{filePath};

    if (!file.is_open())
    {
        std::cerr << "Fail to open config file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        parseLine(line);
    }

    return true;
}

std::string ConfigManager::Get(const std::string& key, const std::string& defaultValue) const
{
    return m_Config.contains(key) ? m_Config.at(key) : defaultValue;
}

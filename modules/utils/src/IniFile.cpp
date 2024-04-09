#include <stdio.h>
#include <string.h>
#include <utils/IniFile.h>

namespace laya
{
IniFile::IniFile(const std::string &filePath)
{
    m_dictionary = iniparser_load(filePath.c_str());
}

IniFile::IniFile()
{
    m_dictionary = dictionary_new(0);
}

IniFile::~IniFile()
{
    if (m_dictionary != nullptr)
    {
        iniparser_freedict(m_dictionary);
    }
}
bool IniFile::hasEntry(const std::string &key) const
{
    return iniparser_find_entry(m_dictionary, key.c_str()) > 0;
}
bool IniFile::saveToFile(const std::string &filePath) const
{
    FILE *pFile;
    pFile = fopen(filePath.c_str(), "w");
    bool const wasFileOpened = pFile != nullptr;
    if (wasFileOpened)
    {
        iniparser_dump_ini(m_dictionary, pFile);
        fclose(pFile);
    }
    return wasFileOpened;
}
bool IniFile::getBoolOrDefault(const std::string &key, bool defaultValue) const
{
    return (bool)iniparser_getboolean(m_dictionary, key.c_str(), defaultValue);
}

int32_t IniFile::getIntOrDefault(const std::string &key, int32_t defaultValue) const
{
    return (int32_t)iniparser_getint(m_dictionary, key.c_str(), defaultValue);
}

uint32_t IniFile::getUIntOrDefault(const std::string &key, uint32_t defaultValue) const
{
    return (uint32_t)iniparser_getint(m_dictionary, key.c_str(), defaultValue);
}

const std::string IniFile::getStringOrDefault(const std::string &key, const std::string &defaultValue) const
{
    return iniparser_getstring(m_dictionary, key.c_str(), defaultValue.c_str());
}

float IniFile::getFloatOrDefault(const std::string &key, float defaultValue) const
{
    return (float)iniparser_getdouble(m_dictionary, key.c_str(), defaultValue);
}

void IniFile::createSection(const std::string &section)
{
    iniparser_set(m_dictionary, section.c_str(), nullptr);
}

void IniFile::setBool(const std::string &key, bool value)
{
    iniparser_set(m_dictionary, key.c_str(), value ? "True" : "False");
}

void IniFile::setInt(const std::string &key, int32_t value)
{
    char buffer[255];
    printf(buffer, 255, "%d", value);
    iniparser_set(m_dictionary, key.c_str(), buffer);
}

void IniFile::setUInt(const std::string &key, uint32_t value)
{
    char buffer[255];
    printf(buffer, 255, "%d", value);
    iniparser_set(m_dictionary, key.c_str(), buffer);
}

void IniFile::setFloat(const std::string &key, float value)
{
    char buffer[255];
    printf(buffer, 255, "%f", value);
    iniparser_set(m_dictionary, key.c_str(), buffer);
}

void IniFile::setString(const std::string &key, const std::string &value)
{
    iniparser_set(m_dictionary, key.c_str(), value.c_str());
}

} // namespace laya
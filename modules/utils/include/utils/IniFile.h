#ifndef __IniFile_H__
#define __IniFile_H__

#include <iniparser/dictionary.h>
#include <iniparser/iniparser.h>
#include <stddef.h>
#include <string>

namespace laya
{
class IniFile
{

  public:
    IniFile();
    IniFile(const std::string &filePath);
    ~IniFile();

    bool hasEntry(const std::string &key) const;
    bool saveToFile(const std::string &filePath) const;
    bool getBoolOrDefault(const std::string &key, bool defaultValue) const;
    int32_t getIntOrDefault(const std::string &key, int32_t defaultValue) const;
    uint32_t getUIntOrDefault(const std::string &key, uint32_t defaultValue) const;
    const std::string getStringOrDefault(const std::string &key, const std::string &defaultValue) const;
    float getFloatOrDefault(const std::string &key, float defaultValue) const;
    void createSection(const std::string &section);
    void setBool(const std::string &key, bool value);
    void setInt(const std::string &key, int32_t value);
    void setUInt(const std::string &key, uint32_t value);
    void setFloat(const std::string &key, float value);
    void setString(const std::string &key, const std::string &value);

  private:
    IniFile(IniFile const &) = delete;
    IniFile &operator=(IniFile const &) = delete;
    IniFile(IniFile &&rhs) = delete;
    IniFile &operator=(IniFile &&rhs) = delete;

  private:
    _dictionary_ *m_dictionary = nullptr;
};
} // namespace laya
#endif
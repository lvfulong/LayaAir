#ifndef __WEBSTORAGE_H__
#define __WEBSTORAGE_H__

#include <sqlite3.h>
#include <optional>
#include <string>
#include <vector>

namespace laya
{
class LocalStorage
{
  public:
    LocalStorage();
    ~LocalStorage();
    void initialize(const std::string &path);
    void setItem(const std::string &key, const std::string &value);
    std::optional<std::string> getItem(const std::string &key);
    void removeItem(const std::string &key);
    void clear();
    int length();
    std::optional<std::string> key(int index);
    std::vector<std::string> keys();

  private:
    sqlite3 *m_db = nullptr;
    sqlite3_stmt *m_stmtSetItem = nullptr;
    sqlite3_stmt *m_stmtGetItem = nullptr;
    sqlite3_stmt *m_stmtRemoveItem = nullptr;
    sqlite3_stmt *m_stmtClear = nullptr;
    sqlite3_stmt *m_stmtLength = nullptr;
    sqlite3_stmt *m_stmtKey = nullptr;
    sqlite3_stmt *m_stmtKeys = nullptr;
};
} // namespace laya
#endif
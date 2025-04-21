#include <utils/JCFileSystem.h>
#include <utils/Log.h>
#include <webstorage/WebStorage.h>

namespace laya
{
LocalStorage::LocalStorage()
{
}
LocalStorage::~LocalStorage()
{
    if (m_stmtKey != nullptr)
    {
        sqlite3_finalize(m_stmtKey);
        m_stmtKey = nullptr;
    }
    if (m_stmtKeys != nullptr)
    {
        sqlite3_finalize(m_stmtKeys);
        m_stmtKeys = nullptr;
    }       
    if (m_stmtLength != nullptr)
    {
        sqlite3_finalize(m_stmtLength);
        m_stmtLength = nullptr;
    }       
    if (m_stmtClear != nullptr)
    {
        sqlite3_finalize(m_stmtClear);
        m_stmtClear = nullptr;
    }
    if (m_stmtRemoveItem != nullptr)
    {
        sqlite3_finalize(m_stmtRemoveItem);
        m_stmtRemoveItem = nullptr;
    }
    if (m_stmtGetItem != nullptr)
    {
        sqlite3_finalize(m_stmtGetItem);
        m_stmtGetItem = nullptr;
    }
    if (m_stmtSetItem != nullptr)
    {
        sqlite3_finalize(m_stmtSetItem);
        m_stmtSetItem = nullptr;
    }
    if (m_db != nullptr)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

void LocalStorage::initialize(const std::string &path)
{
    LOGI("LocalStorage::initialize");
    DEBUG_CHECK(m_db == nullptr);
    std::string dbPath = path + "/localstorage.db";
    bool needCreateTable = !FileSystem::exists(dbPath);

    int ret = sqlite3_open(dbPath.c_str(), &m_db);
    DEBUG_CHECK(ret == SQLITE_OK);
    if (needCreateTable)
    {
        // 设置基本的 PRAGMA
        std::string initial_tweak_pragmas = "PRAGMA journal_mode = WAL;"
                                            "PRAGMA synchronous = NORMAL;"
                                            "PRAGMA temp_store = memory;"
                                            "PRAGMA page_size = 4096;"
                                            "PRAGMA mmap_size = 6000000;"
                                            "PRAGMA optimize;";

        ret = sqlite3_exec(m_db, initial_tweak_pragmas.c_str(), nullptr, nullptr, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        sqlite3_stmt *stmt = nullptr;
        const char *sql = "CREATE TABLE localstorage (key VARCHAR UNIQUE, value VARCHAR)";
        ret = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_step(stmt);
        DEBUG_CHECK(ret == SQLITE_DONE);
        ret = sqlite3_finalize(stmt);
        DEBUG_CHECK(ret == SQLITE_OK);
    }
}

void LocalStorage::setItem(const std::string &key, const std::string &value)
{
    // todo key value 长度限制
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtSetItem == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "INSERT OR REPLACE INTO localstorage (key, value) VALUES (?, ?)", -1, &stmt,
                                 nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtSetItem = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtSetItem);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtSetItem);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtSetItem;
    }
    ret = sqlite3_bind_text(stmt, 1, key.c_str(), key.length(), SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        LOGE("[LocalStorage setItem] sqlite3_bind_text failed: %s", sqlite3_errmsg(m_db));
        return;
    }
    ret = sqlite3_bind_text(stmt, 2, value.c_str(), value.length(), SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        LOGE("[LocalStorage setItem] sqlite3_bind_text failed: %s", sqlite3_errmsg(m_db));
        return;
    }
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_DONE)
    {
        LOGE("[LocalStorage setItem] sqlite3_step failed: %s", sqlite3_errmsg(m_db));
        return;
    }
}

std::optional<std::string> LocalStorage::getItem(const std::string &key)
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtGetItem == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "SELECT value FROM localstorage WHERE key = ?", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtGetItem = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtGetItem);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtGetItem);
        DEBUG_CHECK(ret == SQLITE_OK);

        stmt = m_stmtGetItem;
    }
    ret = sqlite3_bind_text(stmt, 1, key.c_str(), key.length(), SQLITE_STATIC);
    if (ret != SQLITE_OK)
    {
        LOGE("[LocalStorage getItem] sqlite3_bind_text failed: %s", sqlite3_errmsg(m_db));
        return std::nullopt;
    }
    ret = sqlite3_step(stmt);
    if (ret != SQLITE_ROW && ret != SQLITE_DONE)
    {
        LOGE("[LocalStorage getItem] sqlite3_step failed: %s", sqlite3_errmsg(m_db));
        return std::nullopt;
    }
    const char *value = (const char *)sqlite3_column_text(stmt, 0);
    if (value == nullptr)
    {
        return std::nullopt;
    }
    std::string result(value);

    return result;
}
void LocalStorage::removeItem(const std::string &key)
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtRemoveItem == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "DELETE FROM localstorage WHERE key = ?", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtRemoveItem = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtRemoveItem);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtRemoveItem);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtRemoveItem;
    }
    ret = sqlite3_bind_text(stmt, 1, key.c_str(), key.length(), SQLITE_STATIC);
    DEBUG_CHECK(ret == SQLITE_OK);
    ret = sqlite3_step(stmt);
    DEBUG_CHECK(ret == SQLITE_OK || ret == SQLITE_DONE);
}
void LocalStorage::clear()
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtClear == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "DELETE FROM localstorage", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtClear = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtClear);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtClear);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtClear;
    }
    DEBUG_CHECK(ret == SQLITE_OK);
    ret = sqlite3_step(stmt);
    DEBUG_CHECK(ret == SQLITE_OK || ret == SQLITE_DONE);
}
int LocalStorage::length()
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtLength == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "SELECT COUNT(*) FROM localstorage", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtLength = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtLength);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtLength);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtLength;
    }
    ret = sqlite3_step(stmt);
    DEBUG_CHECK(ret == SQLITE_OK || ret == SQLITE_ROW);
    int count = sqlite3_column_int(stmt, 0);    
    DEBUG_CHECK(ret == SQLITE_OK || ret == SQLITE_ROW);
    return count;
}
std::optional<std::string> LocalStorage::key(int index)
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtKey == nullptr)
    {
        ret = sqlite3_prepare_v2(m_db, "SELECT key FROM localstorage LIMIT 1 OFFSET ?", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtKey = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtKey);
        DEBUG_CHECK(ret == SQLITE_OK);
        ret = sqlite3_clear_bindings(m_stmtKey);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtKey;
    }
    ret = sqlite3_bind_int(stmt, 1, index);
    DEBUG_CHECK(ret == SQLITE_OK);
    ret = sqlite3_step(stmt);
    DEBUG_CHECK(ret == SQLITE_OK || ret == SQLITE_ROW || ret == SQLITE_DONE);
    const char *key = (const char *)sqlite3_column_text(stmt, 0);
    if (key == nullptr)
    {
        return std::nullopt;
    }
    std::string result(key);
    return result;
}
std::vector<std::string> LocalStorage::keys()
{
    DEBUG_CHECK(m_db != nullptr);
    int ret = 0;
    sqlite3_stmt *stmt = nullptr;
    if (m_stmtKeys == nullptr)      
    {
        ret = sqlite3_prepare_v2(m_db, "SELECT key FROM localstorage", -1, &stmt, nullptr);
        DEBUG_CHECK(ret == SQLITE_OK);
        m_stmtKeys = stmt;
    }
    else
    {
        ret = sqlite3_reset(m_stmtKeys);
        DEBUG_CHECK(ret == SQLITE_OK);      
        ret = sqlite3_clear_bindings(m_stmtKeys);
        DEBUG_CHECK(ret == SQLITE_OK);
        stmt = m_stmtKeys;
    }

    std::vector<std::string> result;
    while (ret == SQLITE_ROW)
    {
        const char *key = (const char *)sqlite3_column_text(stmt, 0);
        result.push_back(std::string(key));
        ret = sqlite3_step(stmt);
        DEBUG_CHECK(ret == SQLITE_OK);
    }
    return result;
}
} // namespace laya

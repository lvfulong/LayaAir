class JCV8Mgr {
public:
    // ... existing declarations ...

    /**
     * @brief 处理未捕获的异常
     * @param tryCatch V8的TryCatch对象
     */
    void handleUncaughtException(v8::TryCatch& tryCatch);

    /**
     * @brief 执行JavaScript脚本
     * @param script 要执行的脚本内容
     * @return 执行是否成功
     */
    bool executeScript(const char* script);

private:
    // ... existing private members ...
}; 
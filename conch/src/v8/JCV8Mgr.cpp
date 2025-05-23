void JCV8Mgr::initV8() {
    // ... existing initialization code ...

    // 设置未捕获异常处理器
    v8::V8::SetFatalErrorHandler([](const char* location, const char* message) {
        LOGE("V8 Fatal Error at %s: %s", location, message);
    });

    v8::V8::AddMessageListener([](v8::Local<v8::Message> message, v8::Local<v8::Value> error) {
        v8::String::Utf8Value errorStr(v8::Isolate::GetCurrent(), error);
        v8::String::Utf8Value messageStr(v8::Isolate::GetCurrent(), message->Get());
        LOGE("V8 Error: %s\nMessage: %s", *errorStr, *messageStr);
    });

    // 设置未捕获异常处理器
    v8::Isolate::GetCurrent()->SetCaptureStackTraceForUncaughtExceptions(true);
    v8::Isolate::GetCurrent()->AddMessageListener([](v8::Local<v8::Message> message, v8::Local<v8::Value> error) {
        v8::String::Utf8Value errorStr(v8::Isolate::GetCurrent(), error);
        v8::String::Utf8Value messageStr(v8::Isolate::GetCurrent(), message->Get());
        v8::String::Utf8Value stackStr(v8::Isolate::GetCurrent(), message->GetStackTrace()->AsString());
        
        LOGE("Uncaught Exception:\nError: %s\nMessage: %s\nStack: %s", 
            *errorStr, *messageStr, *stackStr);
    });
}

// 添加一个处理未捕获异常的方法
void JCV8Mgr::handleUncaughtException(v8::TryCatch& tryCatch) {
    if (tryCatch.HasCaught()) {
        v8::String::Utf8Value errorStr(v8::Isolate::GetCurrent(), tryCatch.Exception());
        v8::String::Utf8Value messageStr(v8::Isolate::GetCurrent(), tryCatch.Message()->Get());
        v8::String::Utf8Value stackStr(v8::Isolate::GetCurrent(), 
            tryCatch.Message()->GetStackTrace()->AsString());
        
        LOGE("Uncaught Exception:\nError: %s\nMessage: %s\nStack: %s", 
            *errorStr, *messageStr, *stackStr);

        // 可以在这里添加自定义的错误处理逻辑
        // 比如将错误信息发送到服务器，或者保存到日志文件等
    }
}

// 在脚本执行时使用
bool JCV8Mgr::executeScript(const char* script) {
    v8::TryCatch tryCatch(v8::Isolate::GetCurrent());
    
    // 执行脚本
    v8::Local<v8::Script> compiledScript = v8::Script::Compile(
        v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), script).ToLocalChecked()
    );
    
    if (compiledScript.IsEmpty()) {
        handleUncaughtException(tryCatch);
        return false;
    }

    v8::Local<v8::Value> result = compiledScript->Run();
    if (result.IsEmpty()) {
        handleUncaughtException(tryCatch);
        return false;
    }

    return true;
} 
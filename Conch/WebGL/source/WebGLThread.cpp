#include "WebGL/WebGLThread.h"

namespace laya {
WebGLThread::WebGLThread() {
    m_thread = std::thread([this]() {
        do {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                while (!m_requestExit && m_taskQueue.empty()) {
                    m_condition.wait(lock);
                }
                if (m_requestExit && m_taskQueue.empty()) {
                    break;
                }
                task = std::move(m_taskQueue.front());
                m_taskQueue.pop();
            }
            task();
        }while(true);
        
    });
}
WebGLThread::~WebGLThread() {
    requestExit();
}
void WebGLThread::requestExit() {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_requestExit = true;
        m_condition.notify_one();
    }
    if (m_thread.joinable()) {
        m_thread.join();
    }
}
}

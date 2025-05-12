#include <gtest/gtest.h>
#include <network/IUploadTask.h>
#include <utils/Log.h>
#include <thread>
#include <chrono>
#include <fstream>

using namespace laya;

class UploadFileTest : public testing::Test {
protected:
    void SetUp() override {
        // 创建测试文件
        std::ofstream testFile("test_upload.txt");
        testFile << "This is a test file for upload testing.";
        testFile.close();
    }

    void TearDown() override {
        // 清理测试文件
        std::remove("test_upload.txt");
    }
};

// 基本上传测试
TEST_F(UploadFileTest, BasicUpload) {
    auto uploadTask = createUploadTask();
    bool uploadCompleted = false;
    bool uploadSuccess = false;
    std::string errorMessage;

    IUploadTask::Description desc;
    desc.url = "http://localhost:3000/upload";
    desc.filePath = "test_upload.txt";
    desc.headers["Content-Type"] = "multipart/form-data";
    desc.formData["test"] = "value";
    desc.timeout = 30000;

    uploadTask->upload(desc,
        [&](int statusCode, const std::string& data) {
            uploadSuccess = true;
            uploadCompleted = true;
        },
        [&](int statusCode, const std::string& error) {
            errorMessage = error;
            uploadCompleted = true;
        },
        [&]() {
            uploadCompleted = true;
        },
        [&](const std::unordered_map<std::string, std::string>& headers) {
            LOGI("Received headers");
        },
        [&](float progress, int64_t uploaded, int64_t total) {
            LOGI("Upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
        }
    );

    // 等待上传完成
    while (!uploadCompleted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_TRUE(uploadSuccess);
    EXPECT_TRUE(errorMessage.empty());
    delete uploadTask;
}

// 暂停/恢复测试
TEST_F(UploadFileTest, PauseResume) {
    auto uploadTask = createUploadTask();
    bool uploadCompleted = false;
    float lastProgress = 0.0f;
    bool wasPaused = false;

    IUploadTask::Description desc;
    desc.url = "http://localhost:3000/upload";
    desc.filePath = "test_upload.txt";

    uploadTask->upload(desc,
        [&](int statusCode, const std::string& data) {
            uploadCompleted = true;
        },
        [&](int statusCode, const std::string& error) {
            uploadCompleted = true;
        },
        [&]() {
            uploadCompleted = true;
        },
        [&](const std::unordered_map<std::string, std::string>& headers) {
            LOGI("Received headers");
        },
        [&](float progress, int64_t uploaded, int64_t total) {
            lastProgress = progress;
            LOGI("Upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
        }
    );

    // 等待上传开始
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 暂停上传
    uploadTask->pause();
    float progressAtPause = lastProgress;
    wasPaused = true;

    // 等待一段时间
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 验证进度没有变化
    EXPECT_FLOAT_EQ(progressAtPause, lastProgress);

    // 恢复上传
    uploadTask->resume();

    // 等待上传完成
    while (!uploadCompleted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_TRUE(wasPaused);
    EXPECT_GT(lastProgress, progressAtPause);
    delete uploadTask;
}

// 取消上传测试
TEST_F(UploadFileTest, CancelUpload) {
    auto uploadTask = createUploadTask();
    bool uploadCompleted = false;
    bool wasCancelled = false;

    IUploadTask::Description desc;
    desc.url = "http://localhost:3000/upload";
    desc.filePath = "test_upload.txt";

    uploadTask->upload(desc,
        [&](int statusCode, const std::string& data) {
            uploadCompleted = true;
        },
        [&](int statusCode, const std::string& error) {
            wasCancelled = true;
            uploadCompleted = true;
        },
        [&]() {
            uploadCompleted = true;
        },
        [&](const std::unordered_map<std::string, std::string>& headers) {
            LOGI("Received headers");
        },
        [&](float progress, int64_t uploaded, int64_t total) {
            LOGI("Upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
        }
    );

    // 等待上传开始
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 取消上传
    uploadTask->abort();

    // 等待取消完成
    while (!uploadCompleted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_TRUE(wasCancelled);
    delete uploadTask;
}

// 错误处理测试
TEST_F(UploadFileTest, ErrorHandling) {
    auto uploadTask = createUploadTask();
    bool uploadCompleted = false;
    std::string errorMessage;

    IUploadTask::Description desc;
    desc.url = "http://invalid-url/upload";
    desc.filePath = "test_upload.txt";

    uploadTask->upload(desc,
        [&](int statusCode, const std::string& data) {
            uploadCompleted = true;
        },
        [&](int statusCode, const std::string& error) {
            errorMessage = error;
            uploadCompleted = true;
        },
        [&]() {
            uploadCompleted = true;
        },
        [&](const std::unordered_map<std::string, std::string>& headers) {
            LOGI("Received headers");
        },
        [&](float progress, int64_t uploaded, int64_t total) {
            LOGI("Upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
        }
    );

    // 等待上传完成
    while (!uploadCompleted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    EXPECT_FALSE(errorMessage.empty());
    delete uploadTask;
}

// 大文件上传测试
TEST_F(UploadFileTest, LargeFileUpload) {
    // 创建大文件
    const int fileSize = 10 * 1024 * 1024; // 10MB
    std::ofstream largeFile("large_test_upload.txt", std::ios::binary);
    std::vector<char> buffer(1024, 'A');
    for (int i = 0; i < fileSize / 1024; ++i) {
        largeFile.write(buffer.data(), buffer.size());
    }
    largeFile.close();

    auto uploadTask = createUploadTask();
    bool uploadCompleted = false;
    std::vector<float> progressHistory;

    IUploadTask::Description desc;
    desc.url = "http://localhost:3000/upload";
    desc.filePath = "large_test_upload.txt";

    uploadTask->upload(desc,
        [&](int statusCode, const std::string& data) {
            uploadCompleted = true;
        },
        [&](int statusCode, const std::string& error) {
            uploadCompleted = true;
        },
        [&]() {
            uploadCompleted = true;
        },
        [&](const std::unordered_map<std::string, std::string>& headers) {
            LOGI("Received headers");
        },
        [&](float progress, int64_t uploaded, int64_t total) {
            progressHistory.push_back(progress);
            LOGI("Large file upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
        }
    );

    // 等待上传完成
    while (!uploadCompleted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 验证进度记录
    EXPECT_FALSE(progressHistory.empty());
    EXPECT_GE(progressHistory.back(), 1.0f);

    // 清理大文件
    std::remove("large_test_upload.txt");
    delete uploadTask;
}

// 并发上传测试
TEST_F(UploadFileTest, ConcurrentUploads) {
    const int numUploads = 3;
    std::vector<IUploadTask*> tasks;
    std::vector<bool> completed(numUploads, false);
    std::vector<bool> success(numUploads, false);

    // 创建多个上传任务
    for (int i = 0; i < numUploads; ++i) {
        auto task = createUploadTask();
        tasks.push_back(task);
        
        IUploadTask::Description desc;
        desc.url = "http://localhost:3000/upload";
        desc.filePath = "test_upload.txt";

        task->upload(desc,
            [&, i](int statusCode, const std::string& data) {
                completed[i] = true;
                success[i] = true;
            },
            [&](int statusCode, const std::string& error) {
                //errorMessage = error;
                //uploadCompleted = true;
            },
            [&]() {
                //uploadCompleted = true;
            },
            [&](const std::unordered_map<std::string, std::string>& headers) {
                LOGI("Received headers");
            },
            [&](float progress, int64_t uploaded, int64_t total) {
                LOGI("Upload progress: %.2f%% (%lld/%lld bytes)", progress * 100, uploaded, total);
            }
        );
    }

    // 等待所有上传完成
    bool allCompleted = false;
    while (!allCompleted) {
        allCompleted = true;
        for (bool comp : completed) {
            if (!comp) {
                allCompleted = false;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 验证所有上传都成功
    for (bool s : success) {
        EXPECT_TRUE(s);
    }

    // 清理
    for (auto task : tasks) {
        delete task;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

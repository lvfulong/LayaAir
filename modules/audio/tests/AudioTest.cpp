#include <audio/Decoder.h>
#include <audio/AudioPlayer.h>
#include <audio/Audio.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <vector>
#include <string>
#include <utils/JCBuffer.h>
#include <utils/JCFileSystem.h>

using namespace audio;
using namespace laya;
class AudioTest : public testing::Test {
protected:
    void SetUp() override {
        // 每个测试前都会执行
    }

    void TearDown() override {
        // 每个测试后都会执行
    }
};

std::string getExecutablePath()
{
    char szPath[MAX_PATH + 1];
    ::GetModuleFileNameA(NULL, szPath, MAX_PATH + 1);
    std::string result = szPath;
    return result;
}
std::string getAssetRootPath()
{
    std::string exePath = getExecutablePath();
    std::string assetRootPath = laya::FileSystem::parent_path(exePath);
    return assetRootPath;
}
std::string getAssetFullPath(const std::string &assetRelativePath)
{
    return getAssetRootPath() + "/" + assetRelativePath;
}

const std::string& getFilesDir()
{
    static std::string path = getAssetRootPath();
    return path;
}
const std::string& getCacheDir()
{
    static std::string path = getAssetRootPath();
    return path;
}

bool fileNotExists(const char* name)
{
    std::ifstream file(name);
    return !file.good();
}
void loadMp3File(const char* name, std::vector<uint8_t>& dataMp3)
{
    std::string mp3Path = getAssetFullPath(name);

    EXPECT_FALSE(fileNotExists(mp3Path.c_str())) << "file not exists: " << mp3Path;

    JCBuffer buf;
    EXPECT_TRUE(readFileSync(mp3Path.c_str(), buf, JCBuffer::raw)) << "readFileSync failed: " << mp3Path;
    dataMp3.assign(buf.m_pPtr, buf.m_pPtr + buf.m_nLen);
}
// 基本操作测试
TEST_F(AudioTest, BasicOperations) {
    AudioPlayer player;
    Decoder decoder;
    std::vector<uint8_t> dataMp3;
    loadMp3File("test.mp3", dataMp3);
    decoder.loadMp3(dataMp3.data(), dataMp3.size());
    std::shared_ptr<Audio> audio = player.createAudio(&decoder);
    player.play(audio);
    while (true)
    {
        if (audio->isFinished())
        {
                break;
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

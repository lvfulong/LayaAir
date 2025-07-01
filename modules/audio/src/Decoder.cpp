#include <audio/Decoder.h>
#include <cstring>
#include <utils/Log.h>

namespace audio
{
bool Decoder::isMp3Format(const uint8_t *data, size_t size)
{
    if (size < 4)
    {
        return false;
    }

    // 使用memcmp检查ID3v2标记
    if (memcmp(data, "ID3", 3) == 0)
    {
        return true;
    }

    // 检查MPEG同步字 (0xFF 0xFB/0xFA/0xF3/0xF2)
    // 使用位运算优化检查
    return (data[0] == 0xFF) && ((data[1] & 0xE0) == 0xE0);
}

bool Decoder::isWavFormat(const uint8_t *data, size_t size)
{
    if (size < 16) // 至少需要检查RIFF和WAVE标记
    {
        return false;
    }

    // 使用memcmp检查RIFF标记
    if (memcmp(data, "RIFF", 4) != 0)
    {
        return false;
    }

    // 使用memcmp检查WAVE标记
    if (memcmp(data + 8, "WAVE", 4) != 0)
    {
        return false;
    }

    // 使用memcmp检查fmt标记
    if (memcmp(data + 12, "fmt ", 4) != 0)
    {
        return false;
    }

    return true;
}

bool Decoder::isOggFormat(const uint8_t *data, size_t size)
{
    if (size < 32) // Ogg页头+最小包体
        return false;

    // 检查OggS标记
    if (memcmp(data, "OggS", 4) != 0)
        return false;

    // 检查版本号
    if (data[4] != 0)
        return false;

    // 检查包头类型
    if ((data[5] & 0x07) == 0)
        return false;

    // 检查segment table长度
    uint8_t segmentCount = data[26];
    if (size < 27 + segmentCount)
        return false;

    // 计算第一个包的起始位置
    size_t packetStart = 27 + segmentCount;
    if (size < packetStart + 7)
        return false;

    // 检查是否包含"vorbis"标识
    if (memcmp(data + packetStart + 1, "vorbis", 6) == 0)
        return true;

    return false;
}
} // namespace audio

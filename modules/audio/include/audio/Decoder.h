#ifndef __DECODER_H__
#define __DECODER_H__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utils/Log.h>

namespace audio
{
class Decoder : public std::enable_shared_from_this<Decoder>
{
  public:
    enum Type
    {
        TYPE_STATIC,
        TYPE_STREAM,
        TYPE_UNKNOWN,
    };
    Decoder() = default;
    virtual ~Decoder() = default;
    virtual bool canDecode(int8_t *data, size_t size) = 0;
    virtual bool load(const uint8_t *data, size_t size) = 0;
    size_t getSampleCount() const
    {
        return m_sampleCount;
    }
    size_t getChannelCount() const
    {
        return m_channelCount;
    }
    float getSampleRate() const
    {
        return m_sampleRate;
    }
    size_t getSize() const
    {
        return m_size;
    }
    size_t getBitPerSample() const
    {
        return m_bitPerSample;
    }
    Type getType() const
    {
        return m_type;
    }
    virtual double getDuration() const
    {
        return m_sampleCount / m_sampleRate;
    }
    static bool isMp3Format(const uint8_t *data, size_t size);
    static bool isWavFormat(const uint8_t *data, size_t size);
    static bool isOggFormat(const uint8_t *data, size_t size);

  protected:
    size_t m_sampleCount{0};
    size_t m_channelCount{0};
    float m_sampleRate{0};
    size_t m_bitPerSample{16};
    size_t m_size{0};
    Type m_type{TYPE_UNKNOWN};
};

} // namespace audio
#endif
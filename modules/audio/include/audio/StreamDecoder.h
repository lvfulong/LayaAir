#ifndef __STREAM_DECODER_H__
#define __STREAM_DECODER_H__

#include <audio/Decoder.h>
#include <cstddef>
#include <vector>
#include <memory>

namespace audio
{
class StreamDecoder : public Decoder
{
  public:
    StreamDecoder()
    {
        m_type = TYPE_STREAM;
    }
    virtual ~StreamDecoder() = default;

    virtual size_t getAudio(uint8_t *buffer, size_t bufferSize) = 0; 
    virtual bool seek(size_t seconds) = 0;
    virtual bool rewind() = 0;
    virtual bool isFinished() = 0;
    virtual double tell() const = 0;  // 获取当前位置（秒）
    virtual void setLoop(bool enable)
    {
      m_loop = enable;
    }
    virtual bool getLoop() const
    {
      return m_loop;
    }
    static std::shared_ptr<StreamDecoder> create(const std::string &url, const uint8_t *data, size_t dataSize);
  protected:
    bool m_loop{false};
};
} // namespace audio
#endif
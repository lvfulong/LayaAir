#ifndef __STREAM_DECODER_MP3_H__
#define __STREAM_DECODER_MP3_H__

#include "dr_mp3.h"
#include <audio/StreamDecoder.h>
#include <cstddef>
#include <vector>
#include <utils/Data.h>

namespace audio
{
class StreamDecoderMp3 : public StreamDecoder
{
  public:
    StreamDecoderMp3() = default;
    ~StreamDecoderMp3();
    virtual bool canDecode(int8_t *data, size_t size) override;
    virtual bool load(const std::shared_ptr<laya::Data>& data) override;
    virtual size_t getAudio(uint8_t *buffer, size_t bufferSize) override;
    virtual bool seek(size_t seconds) override;
    virtual bool rewind() override;
    virtual bool isFinished() override;
    virtual double tell() const override;
  private:
    drmp3* m_decoder{nullptr};
    bool m_eof{false};
    std::shared_ptr<laya::Data> m_data;//流式解码器需要引用数据，否则数据会被释放
};
} // namespace audio
#endif
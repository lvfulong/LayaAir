#ifndef __STREAM_DECODER_MP3_H__
#define __STREAM_DECODER_MP3_H__

#include "dr_mp3.h"
#include <audio/StreamDecoder.h>
#include <cstddef>
#include <vector>

namespace audio
{
class StreamDecoderMp3 : public StreamDecoder
{
  public:
    StreamDecoderMp3() = default;
    ~StreamDecoderMp3();
    virtual bool canDecode(int8_t *data, size_t size) override;
    virtual bool load(const uint8_t *data, size_t size) override;
    virtual size_t getAudio(uint8_t *buffer, size_t bufferSize) override;
    virtual bool seek(size_t seconds) override;
    virtual bool rewind() override;
    virtual bool isFinished() override;
    virtual double tell() const override;
  private:
    drmp3* m_decoder{nullptr};
    bool m_eof{false};
    uint8_t *m_buffer{nullptr};
};
} // namespace audio
#endif
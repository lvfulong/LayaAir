#ifndef __STREAM_DECODER_OGG_H__
#define __STREAM_DECODER_OGG_H__

#include <audio/StreamDecoder.h>
#include <cstddef>
#include <vector>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

namespace audio
{
class StreamDecoderOgg : public StreamDecoder
{
  public:
    StreamDecoderOgg() = default;
    ~StreamDecoderOgg();
    virtual bool canDecode(int8_t *data, size_t size) override;
    virtual bool load(const uint8_t *data, size_t size) override;
    virtual size_t getAudio(uint8_t *buffer, size_t bufferSize) override;
    virtual bool seek(size_t seconds) override;
    virtual bool rewind() override;
    virtual bool isFinished() override;
    virtual double tell() const override;
  private:
    OggVorbis_File* m_decoder{nullptr};
    bool m_eof{false};
    uint8_t *m_buffer{nullptr};
    size_t m_bufferSize{0};
    vorbis_info* m_vorbisInfo{nullptr};
};
} // namespace audio
#endif
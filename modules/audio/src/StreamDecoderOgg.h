#ifndef __STREAM_DECODER_OGG_H__
#define __STREAM_DECODER_OGG_H__

#include <audio/StreamDecoder.h>
#include <cstddef>
#include <vector>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <utils/Data.h>

namespace audio
{
class StreamDecoderOgg : public StreamDecoder
{
  public:
    StreamDecoderOgg() = default;
    ~StreamDecoderOgg();
    virtual bool canDecode(int8_t *data, size_t size) override;
    virtual bool load(const std::shared_ptr<laya::Data>& data) override;
    virtual size_t getAudio(uint8_t *buffer, size_t bufferSize) override;
    virtual bool seek(size_t seconds) override;
    virtual bool rewind() override;
    virtual bool isFinished() override;
    virtual double tell() const override;
  private:
    OggVorbis_File* m_decoder{nullptr};
    bool m_eof{false};
    vorbis_info* m_vorbisInfo{nullptr};
    std::shared_ptr<laya::Data> m_data;//流式解码器需要引用数据，否则数据会被释放
};
} // namespace audio
#endif
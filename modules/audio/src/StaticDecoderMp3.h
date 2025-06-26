#ifndef __STATIC_DECODER_MP3_H__
#define __STATIC_DECODER_MP3_H__

#include <audio/StaticDecoder.h>
#include <cstddef>
#include <cstdint>

namespace audio
{
class StaticDecoderMp3 : public StaticDecoder
{
  public:
    StaticDecoderMp3() = default;
    ~StaticDecoderMp3() = default;
    bool canDecode(int8_t *data, size_t size) override;
    bool load(const std::shared_ptr<laya::Data>& data) override;
};
} // namespace audio
#endif
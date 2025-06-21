#ifndef __STATIC_DECODER_WAV_H__
#define __STATIC_DECODER_WAV_H__

#include <audio/StaticDecoder.h>
#include <cstddef>
#include <cstdint>

namespace audio
{
class StaticDecoderWav : public StaticDecoder
{
  public:
    StaticDecoderWav() = default;
    ~StaticDecoderWav() = default;
    bool canDecode(int8_t *data, size_t size) override;
    bool load(const uint8_t *data, size_t size) override;
};
} // namespace audio
#endif
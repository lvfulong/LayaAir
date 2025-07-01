#ifndef __STATIC_DECODER_H__
#define __STATIC_DECODER_H__

#include <audio/Decoder.h>
#include <cstddef>
#include <cstdint>

namespace audio
{
class StaticDecoder : public Decoder
{
  public:
    StaticDecoder() : m_data(nullptr)
    {
        m_type = TYPE_STATIC;
    }
    virtual ~StaticDecoder()
    {
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }
    uint16_t *getData() const
    {
        return m_data;
    }

  protected:
    uint16_t *m_data;
};
} // namespace audio
#endif
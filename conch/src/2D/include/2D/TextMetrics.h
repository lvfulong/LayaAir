#ifndef __TextMetrics_H__
#define __TextMetrics_H__
#include <memory>
#include <string>
namespace laya
{
class TextMetrics
{
  public:
    TextMetrics() = default;
    TextMetrics(TextMetrics const &) = default;
    TextMetrics &operator=(TextMetrics const &) = default;

    TextMetrics(TextMetrics &&) = default;
    TextMetrics &operator=(TextMetrics &&) = default;
    float m_width{0};
    float m_height{0};
    float m_ascender{0};
    float m_descender{0};
};
} // namespace laya
#endif

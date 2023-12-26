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
    int m_width{0};
    int m_height{0};
    int m_ascender{0};
};
} // namespace laya
#endif
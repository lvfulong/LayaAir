#ifndef __ImageData_H__
#define __ImageData_H__
#include <string>
#include <vector>
#include <cstdint>
namespace laya
{
class ImageData
{
  public:
    ImageData() = default;
    ImageData(ImageData const &) = delete;
    ImageData &operator=(ImageData const &) = delete;

    ImageData(ImageData &&) = default;
    ImageData &operator=(ImageData &&) = default;

    int m_width{0};
    int m_height{0};
    std::vector<uint8_t> m_data;
};
} // namespace laya
#endif
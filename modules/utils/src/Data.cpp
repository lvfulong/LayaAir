#include "utils/Data.h"
#include <cstring>

namespace laya
{
std::shared_ptr<Data> Data::makeFromFile(const std::string &filePath)
{
    // todo
    return std::shared_ptr<Data>(nullptr);
}

std::shared_ptr<Data> Data::makeWithCopy(const void *bytes, size_t length)
{
    if (length == 0)
    {
        return makeEmpty();
    }
    auto data = new (std::nothrow) uint8_t[length];
    if (data == nullptr)
    {
        return nullptr;
    }
    memcpy(data, bytes, length);
    auto byteData = new Data(data, length, Data::DeleteProc, nullptr);
    return std::shared_ptr<Data>(byteData);
}

std::shared_ptr<Data> Data::makeWithoutCopy(const void *data, size_t length)
{
    if (length == 0)
    {
        return makeEmpty();
    }
    auto byteData = new Data(data, length, nullptr, nullptr);
    return std::shared_ptr<Data>(byteData);
}

void Data::DeleteProc(const void *data, void *)
{
    if (data != nullptr)
    {
        delete[] reinterpret_cast<const uint8_t *>(data);
    }
}

void Data::FreeProc(const void *data, void *)
{
    if (data != nullptr)
    {
        free(const_cast<void *>(data));
    }
}

std::shared_ptr<Data> Data::makeAdopted(const void *data, size_t length, ReleaseProc releaseProc, void *context)
{
    if (length == 0)
    {
        return makeEmpty();
    }
    auto byteData = new Data(data, length, releaseProc, context);
    return std::shared_ptr<Data>(byteData);
}

std::shared_ptr<Data> Data::makeEmpty()
{
    static auto emptyData = std::shared_ptr<Data>(new Data(nullptr, 0, nullptr, nullptr));
    return emptyData;
}

Data::Data(const void *data, size_t length, ReleaseProc releaseProc, void *context)
    : m_data(data), m_size(length), releaseProc(releaseProc), releaseContext(context)
{
}

Data::~Data()
{
    if (releaseProc)
    {
        releaseProc(m_data, releaseContext);
    }
}

} // namespace laya

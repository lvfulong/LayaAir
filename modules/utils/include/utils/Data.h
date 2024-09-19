#ifndef __DATA_H__
#define __DATA_H__

#include <memory>
#include <string>

namespace laya
{

class Data
{
  public:
    static std::shared_ptr<Data> makeFromFile(const std::string &filePath);
    static std::shared_ptr<Data> makeWithCopy(const void *data, size_t length);
    static std::shared_ptr<Data> makeWithoutCopy(const void *data, size_t length);
    typedef void (*ReleaseProc)(const void *data, void *context);

    static void DeleteProc(const void *data, void *context);
    static void FreeProc(const void *data, void *context);

    static std::shared_ptr<Data> makeAdopted(const void *data, size_t length, ReleaseProc releaseProc = DeleteProc,
                                             void *context = nullptr);

    static std::shared_ptr<Data> makeEmpty();

    ~Data();

    const void *data() const
    {
        return m_data;
    }

    const uint8_t *bytes() const
    {
        return reinterpret_cast<const uint8_t *>(m_data);
    }

    size_t size() const
    {
        return m_size;
    }

    bool empty() const
    {
        return m_size == 0;
    }

  private:
    const void *m_data = nullptr;
    size_t m_size = 0;
    ReleaseProc releaseProc = nullptr;
    void *releaseContext = nullptr;

    Data(const void *data, size_t length, ReleaseProc releaseProc, void *context);
};

} // namespace laya
#endif

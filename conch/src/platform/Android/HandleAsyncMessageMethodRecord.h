
#ifndef __HandleAsyncMessageMethodRecord_H__
#define __HandleAsyncMessageMethodRecord_H__

#include <functional>
#include <string>

namespace laya
{

class HandleAsyncMessageMethodRecord
{
  public:
    std::function<void(std::string)> m_callback;
};
} // namespace laya
#endif

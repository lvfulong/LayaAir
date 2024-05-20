#ifndef __EngineEvent_H__
#define __EngineEvent_H__

namespace laya
{
enum class EngineEventType
{
    Reload,
    UrlBack,
};
class EngineEventBase
{
  public:
    virtual ~EngineEventBase()
    {
    }
    EngineEventBase(EngineEventType type) : m_type(type)
    {
    }
    EngineEventType m_type;
};

class EngineEventReload : public EngineEventBase
{
  public:
    EngineEventReload() : EngineEventBase(EngineEventType::Reload)
    {
    }
};
class EngineEventUrlBack : public EngineEventBase
{
  public:
    EngineEventUrlBack() : EngineEventBase(EngineEventType::UrlBack)
    {
    }
};
} // namespace laya
#endif
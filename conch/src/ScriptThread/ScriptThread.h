#ifndef __SCRIPT_THREAD_H__
#define __SCRIPT_THREAD_H__

namespace laya
{
class ScriptThread: public laya::JCWorkerThread
{
  public:
    virtual ~ScriptThread()
    {
    }
    ScriptThread()
    {
    }
        // 因为要trycatch，所以只好再抄一份了
    virtual void _defRunLoop();

    virtual void _runLoop();

  public:
    Javascript *m_pJS;
};
} // namespace laya
#endif
#ifndef __Disposable_H__
#define __Disposable_H__

namespace laya
{
	class Disposable
	{
	public:
		Disposable() = default;
		virtual ~Disposable()
		{
			destroy();
		}
		virtual void destroy()
		{
			if (m_destroyed) 
				return;
			doDestroy();
			m_destroyed = true;
		}
		virtual void doDestroy() = 0;
	protected:
		bool m_destroyed = false;
	};
}
#endif
#ifndef __GLObject_H__
#define __GLObject_H__

#include "render/RenderDriver/OpenGLESDriver/RenderDevice/GLESEngine.h"
#include <render/3D/temp/ObjectBase.h>

namespace laya
{
	class GLObject//: public ObjectBase
	{
	public:
		GLObject(GLESEngine* engine);
		virtual ~GLObject();
		/*bool destroyed()
		{
			return m_destroyed;
		}
		virtual void setResourceManager() {};

		virtual void destroy()
		{
			if (m_destroyed) 
				return;
			m_destroyed = true;
		}*/
	protected:
		GLESEngine* m_engine = nullptr;
		//int m_id;
		bool m_destroyed = false;
	};
}
#endif
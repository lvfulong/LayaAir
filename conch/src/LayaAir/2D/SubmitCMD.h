#ifndef __SubmitCMD_H__
#define __SubmitCMD_H__

#include <stack>
#include "BlendMode.h"
#include "SubmitBase.h"
#include "Matrix.h"
#include <functional>
namespace laya
{
	class SubmitCMD : public SubmitBase
	{	
	public:
		static SubmitCMD* create(std::function<void()> func);
        static void onDestroy();
		SubmitCMD(GLESEngine* pWebglEngine);
		int renderSubmit() override;
		void recycle() override;
		float u_TexRange[4];
	private:
		bool m_renderTarget = false;
		static std::stack<SubmitCMD*> POOL;
		std::function<void()> m_func;
	};
}
#endif //__SubmitCMD_H__

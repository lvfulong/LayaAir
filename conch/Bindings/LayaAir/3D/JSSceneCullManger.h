#ifndef __JSSceneCullManager_H__
#define __JSSceneCullManager_H__

#include <stdio.h>
#include <binder/JSInterface.h>
#include "JSRenderNode.h"
#include <utils/JCSingletonList.h>

namespace laya
{
	class JSSceneCullManger
	{
	public:
		static void exportJS(Context& context);
		JSSceneCullManger();
		~JSSceneCullManger();

		void addRenderObject(JSValueAsParam object);
		
		void removeRenderObject(JSValueAsParam object);
		
		void removeMotionObject(JSValueAsParam object);
		
		void addMotionObject(JSValueAsParam object);
		
		void updateMotionObjects();
		
		void destroy();

		void clear();

	public:
		
		JCSimpleSingletonList*				m_pNodeList;
		//���Motion��ʹ��SimpleList����Ϊ��ͬһ��RenderNode����ͬʱ��������list�У�listID�����
		JCSingletonList<JSRenderNode*>*		m_pMotionList;

	};
}
#endif //__JSSceneCullManager_H__
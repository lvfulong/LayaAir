#ifndef __Path_H__
#define __Path_H__

#include <stdio.h>
#include <map>
#include <vector>
#include <stack>
#include <utils/Preprocessor.h>
#include <cstring>
namespace laya
{
	struct RenderPath
	{
		std::vector<float> m_path; //[x,y,x,y,....]������
		bool m_loop;
		bool m_convex;
		RenderPath(bool loop, bool convex) :
			m_loop(loop),
			m_convex(convex)
		{
			m_path.reserve(128);
		}

	};
	class Path
	{
	public:
		Path() {}
		~Path() {}
		void beginPath(bool convex);
		void newPath();
		void addPoint(float pointX, float pointY);
		void push(const std::vector<float>& points, bool convex);
		void reset();
		void closePath();
		std::vector<RenderPath> m_paths;
		float m_lastOriX = 0.0f;	//moveto�ȵ�ԭʼλ�á�û�о����ڲ�����任��
		float m_lastOriY = 0.0f;
		RenderPath* m_curPath;
	}; 
}
#endif //__Path_H__

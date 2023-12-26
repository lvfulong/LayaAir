#ifndef __Bezier_H__
#define __Bezier_H__

#include <stdio.h>
#include <vector>
#include <deque>
#include "Point.h"

namespace laya
{
	class Bezier
	{
	private:
		std::deque<Point> m_controlPoints{ Point(), Point(), Point() };
		//_calFun: Function = this.getPoint2;
		int m_count;
		void _switchPoint(float x, float y);

		/**
		* 计算二次贝塞尔点。
		*/
		void getPoint2(float t, std::vector<float>& rst);

		/**
		* 计算三次贝塞尔点
		*/
		void getPoint3(float t, std::vector<float>& rst);
		

		/**
		* 计算贝塞尔点序列
		*/
		void insertPoints(float count, std::vector<float>& rst);
		
	public:
		/**
		* 获取贝塞尔曲线上的点。
		* @param pList 控制点[x0,y0,x1,y1...]
		* @param inSertCount 每次曲线的插值数量
		*/
		void getBezierPoints(const std::vector<float>& pList, int inSertCount/* = 5*/, int count/* = 2*/, std::vector<float>& out);
		
	};
}
#endif

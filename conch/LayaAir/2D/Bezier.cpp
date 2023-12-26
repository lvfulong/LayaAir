#include "Bezier.h"

namespace laya
{
	void Bezier::_switchPoint(float x, float y)
	{
		Point& tPoint = m_controlPoints.front();
		tPoint.setTo(x, y);
		m_controlPoints.push_back(tPoint);
		m_controlPoints.pop_front();
	}

	/**
	* 计算二次贝塞尔点。
	*/
	void Bezier::getPoint2(float t, std::vector<float>& rst)
	{
		//二次贝塞尔曲线公式
		Point& p1 = m_controlPoints[0];
		Point& p2 = m_controlPoints[1];
		Point& p3 = m_controlPoints[2];
		float lineX = pow((1 - t), 2) * p1.x + 2 * t * (1 - t) * p2.x + pow(t, 2) * p3.x;
		float lineY = pow((1 - t), 2) * p1.y + 2 * t * (1 - t) * p2.y + pow(t, 2) * p3.y;
		rst.push_back(lineX);
		rst.push_back(lineY);
	}

	/**
	* 计算三次贝塞尔点
	*/
	void Bezier::getPoint3(float t, std::vector<float>& rst)
	{
		//三次贝塞尔曲线公式
		Point& p1 = m_controlPoints[0];
		Point& p2 = m_controlPoints[1];
		Point& p3 = m_controlPoints[2];
		Point& p4 = m_controlPoints[3];
		float lineX = pow((1 - t), 3) * p1.x + 3 * p2.x * t * (1 - t) * (1 - t) + 3 * p3.x * t * t * (1 - t) + p4.x * pow(t, 3);
		float lineY = pow((1 - t), 3) * p1.y + 3 * p2.y * t * (1 - t) * (1 - t) + 3 * p3.y * t * t * (1 - t) + p4.y * pow(t, 3);
		rst.push_back(lineX);
		rst.push_back(lineY);
	}

	/**
	* 计算贝塞尔点序列
	*/
	void Bezier::insertPoints(float count, std::vector<float>& rst)
	{
		float i;
		count = count > 0 ? count : 5;
		float dLen;
		dLen = 1.0f / count;
		for (i = 0; i <= 1; i += dLen)
		{
			switch (m_count)
			{
			case 2:
				getPoint2(i, rst);
				break;
			case 3:
				getPoint3(i, rst);
				break;
			default:
				//assert(true);
				rst.clear();
			}
		}
	}
	void Bezier::getBezierPoints(const std::vector<float>& pList, int inSertCount/* = 5*/, int count/* = 2*/, std::vector<float>& out)
	{
		int i, len;
		len = pList.size();
		if (len < (count + 1) * 2)
		{
			out.clear();
			return;
		}
		out.clear();
		/*switch (count)
		{
		case 2:
		this._calFun = this.getPoint2;
		break;
		case 3:
		this._calFun = this.getPoint3;
		break;
		default:
		return[];
		}*/
		m_count = count;
		while (m_controlPoints.size() <= count)
		{
			m_controlPoints.emplace_back(Point());
		}
		for (int i = 0; i < count * 2; i += 2)
		{
			_switchPoint(pList[i], pList[i + 1]);
		}
		for (int i = count * 2; i < len; i += 2)
		{
			_switchPoint(pList[i], pList[i + 1]);
			if ((i / 2) % count == 0)
				insertPoints(inSertCount, out);
		}
	}
}
#include "Path.h"


namespace laya
{
	void Path::beginPath(bool convex)
	{
		m_paths.clear();
		m_paths.emplace_back(false, convex);
		m_curPath = &m_paths[0];
		m_curPath->m_convex = convex;
	}
	void Path::newPath()
	{
		m_paths.emplace_back(false, false);
		m_curPath = &m_paths.back();
	}
	void Path::addPoint(float pointX, float pointY)
	{
		m_curPath->m_path.push_back(pointX);
		m_curPath->m_path.push_back(pointY);
	}
	void Path::push(const std::vector<float>& points, bool convex)
	{
		if (!m_curPath)
		{
			m_paths.emplace_back(false, false);
			m_curPath = &m_paths.back();
		}
		else if (m_curPath->m_path.size() > 0)
		{
			m_paths.emplace_back(false, false);
			m_curPath = &m_paths.back();
		}
		m_curPath->m_path.resize(points.size());
		memcpy(&m_curPath->m_path[0], &points[0], sizeof(float) * points.size());
		m_curPath->m_convex = convex;
	}
	void Path::reset()
	{
		m_paths.clear();
		m_curPath = nullptr;
	}
	void Path::closePath()
	{
		m_curPath->m_loop = true;
	}
}
//------------------------------------------------------------------------------

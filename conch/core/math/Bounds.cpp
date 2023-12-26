#include "Bounds.h"
#include <utils/Log.h>
#include <utils/JCMemorySurvey.h>
#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

namespace laya
{
	//------------------------------------------------------------------------------
	Bounds::Bounds()
	{
	}
	//------------------------------------------------------------------------------
	Bounds::~Bounds()
	{
	}

	void Bounds::setMin(const Vector3& value)
	{
		m_boundBox.min = value;
		_setUpdateFlag(Bounds::_UPDATE_CENTER | Bounds::_UPDATE_EXTENT, true);
		_setUpdateFlag(Bounds::_UPDATE_MIN, false);
	}

	const Vector3& Bounds::getMin() const
	{
		Vector3& min = m_boundBox.min;
		if (_getUpdateFlag(Bounds::_UPDATE_MIN)) 
		{
			_getMin(getCenter(), getExtent(), min);
			_setUpdateFlag(Bounds::_UPDATE_MIN, false);
		}
		return min;
	}

	void Bounds::setMax(const Vector3& value)
	{
		Vector3& max = m_boundBox.max = value;
		_setUpdateFlag(Bounds::_UPDATE_CENTER | Bounds::_UPDATE_EXTENT, true);
		_setUpdateFlag(Bounds::_UPDATE_MAX, false);
	}

	const Vector3& Bounds::getMax() const
	{
		Vector3& max = m_boundBox.max;
		if (_getUpdateFlag(Bounds::_UPDATE_MAX))
		{
			_getMax(getCenter(), getExtent(), max);
			_setUpdateFlag(Bounds::_UPDATE_MAX, false);
		}
		return m_boundBox.max;
	}

	void Bounds::setCenter(const Vector3& value)
	{
		m_center = value;
		_setUpdateFlag(Bounds::_UPDATE_MIN | Bounds::_UPDATE_MAX, true);
		_setUpdateFlag(Bounds::_UPDATE_CENTER, false);
	}

	const Vector3& Bounds::getCenter() const
	{
		if (_getUpdateFlag(Bounds::_UPDATE_CENTER))
		{
			_getCenter(getMin(), getMax(), m_center);
			_setUpdateFlag(Bounds::_UPDATE_CENTER, false);
		}
		return m_center;
	}

	void Bounds::setExtent(const Vector3& value)
	{
		m_extent = value;
		_setUpdateFlag(Bounds::_UPDATE_MIN | Bounds::_UPDATE_MAX, true);
		_setUpdateFlag(Bounds::_UPDATE_EXTENT, false);
	}

	const Vector3& Bounds::getExtent() const
	{
		if (_getUpdateFlag(Bounds::_UPDATE_EXTENT))
		{
			_getExtent(getMin(), getMax(), m_extent);
			_setUpdateFlag(Bounds::_UPDATE_EXTENT, false);
		}
		return m_extent;
	}

	Bounds::Bounds(const Vector3& min, const Vector3& max)
	{
		m_boundBox.min = min;
		m_boundBox.max = max;
		_setUpdateFlag(Bounds::_UPDATE_CENTER | Bounds::_UPDATE_EXTENT, true);
	}

	bool Bounds::_getUpdateFlag(uint64_t type) const
	{
		return (m_updateFlag & type) != 0;
	}


	void Bounds::_setUpdateFlag(uint64_t type, bool value) const
	{
		if (value)
			m_updateFlag |= type;
		else
			m_updateFlag &= ~type;
	}

	void Bounds::_getCenter(const Vector3& min, const Vector3& max, Vector3& out) const
	{
		Vector3::add(min, max, out);
		Vector3::scale(out, 0.5f, out);
	}

	void Bounds::_getExtent(const Vector3& min, const Vector3& max, Vector3& out) const
	{
		Vector3::subtract(max, min, out);
		Vector3::scale(out, 0.5f, out);
	}

	void Bounds::_getMin(const Vector3& center, const Vector3& extent, Vector3& out) const
	{
		Vector3::subtract(center, extent, out);
	}

	void Bounds::_getMax(const Vector3& center, const Vector3& extent, Vector3& out) const
	{
		Vector3::add(center, extent, out);
	}

	void Bounds::_rotateExtents(const Vector3& extents, const Matrix4x4& rotation, Vector3& out)
	{
		const float* matE = rotation.elements;
		out.x = abs(matE[0] * extents.x) + abs(matE[4] * extents.y) + abs(matE[8] * extents.z);
		out.y = abs(matE[1] * extents.x) + abs(matE[5] * extents.y) + abs(matE[9] * extents.z);
		out.z = abs(matE[2] * extents.x) + abs(matE[6] * extents.y) + abs(matE[10] * extents.z);
	}

	void Bounds::_tranform(const Matrix4x4& matrix, Bounds& out)
	{
		Vector3& outCen = out.m_center;
		Vector3& outExt = out.m_extent;

		Vector3::transformCoordinate(getCenter(), matrix, outCen);
		_rotateExtents(getExtent(), matrix, outExt);

		out.m_boundBox.setCenterAndExtent(outCen, outExt);
		out.m_updateFlag = 0;
	}

	const BoundBox& Bounds::_getBoundBox()
	{
		if (m_updateFlag & Bounds::_UPDATE_MIN) 
		{
			Vector3& min = m_boundBox.min;
			_getMin(getCenter(), getExtent(), min);
			_setUpdateFlag(Bounds::_UPDATE_MIN, false);
		}
		if (m_updateFlag & Bounds::_UPDATE_MAX)
		{
			Vector3& max = m_boundBox.max;
			_getMax(getCenter(), getExtent(), max);
			_setUpdateFlag(Bounds::_UPDATE_MAX, false);
		}
		return m_boundBox;
	}

	float Bounds::calculateBoundsintersection(const Bounds& bounds) const 
	{
		const Vector3& ownMax = getMax();
		const Vector3& ownMin = getMin();
		const Vector3& calMax = bounds.getMax();
		const Vector3& calMin = bounds.getMin();
		Vector3 tempV0;// = TEMP_VECTOR3_MAX0;
		Vector3 tempV1;// = TEMP_VECTOR3_MAX1;
		const Vector3& thisExtends = getExtent();
		const Vector3& boundExtends = bounds.getExtent();
		tempV0.setValue(std::max(ownMax.x, calMax.x) - std::min(ownMin.x, calMin.x),
			std::max(ownMax.y, calMax.y) - std::min(ownMin.y, calMin.y),
			std::max(ownMax.z, calMax.z) - std::min(ownMin.z, calMin.z));
		tempV1.setValue((thisExtends.x + boundExtends.x) * 2.0f,
			(thisExtends.y + boundExtends.y) * 2.0f,
			(thisExtends.z + boundExtends.z) * 2.0f);
		if ((tempV0.x) > (tempV1.x)) return -1;
		if ((tempV0.y) > (tempV1.y)) return -1;
		if ((tempV0.z) > (tempV1.z)) return -1;
		return (tempV1.x - tempV0.x) * (tempV1.y - tempV0.y) * (tempV1.z - tempV0.z);
	}
	void Bounds::cloneTo(Bounds& destBounds)
	{
		destBounds.m_boundBox.min = getMin();
		destBounds.m_boundBox.max = getMax();
		destBounds.m_center = getCenter();
		destBounds.m_extent = getExtent();
		destBounds.m_updateFlag = 0;
	}
}
//------------------------------------------------------------------------------

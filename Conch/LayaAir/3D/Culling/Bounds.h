#ifndef __Bounds_H__
#define __Bounds_H__

#include <stdio.h>
#include "LayaAir/3D/Math/BoundBox.h"

namespace laya
{
	class Bounds
	{
	public:
		Bounds();
		~Bounds();

		void setMin(const Vector3& value);
		const Vector3& getMin() const;
		void setMax(const Vector3& value);
		const Vector3& getMax() const;
		void setCenter(const Vector3& value);
		const Vector3& getCenter() const;
		void setExtent(const Vector3& value);
		const Vector3& getExtent() const;
		Bounds(const Vector3& min, const Vector3& max);
		bool _getUpdateFlag(uint64_t type) const;
		void _setUpdateFlag(uint64_t type, bool value) const;
		void _getCenter(const Vector3& min, const Vector3& max, Vector3& out) const;
		void _getExtent(const Vector3& min, const Vector3& max, Vector3& out) const;
		void _getMin(const Vector3& center, const Vector3& extent, Vector3& out) const;
		void _getMax(const Vector3& center, const Vector3& extent, Vector3& out) const;
		void _rotateExtents(const Vector3& extents, const Matrix4x4& rotation, Vector3& out);
		void _tranform(const Matrix4x4& matrix, Bounds& out);
		const BoundBox& _getBoundBox();
		float calculateBoundsintersection(const Bounds& bounds) const;
		void cloneTo(Bounds& destBounds);

	public:
		mutable BoundBox				m_boundBox;
		mutable Vector3							m_center;
		mutable Vector3							m_extent;
		mutable uint64_t						m_updateFlag = 0;
		
		enum
		{
			_UPDATE_MIN = 0x01,
			_UPDATE_MAX = 0x02,
			_UPDATE_CENTER = 0x04,
		    _UPDATE_EXTENT = 0x08,
		};
	};
}
#endif //__Bounds_H__
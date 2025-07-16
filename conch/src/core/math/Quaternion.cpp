#include "Quaternion.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
namespace laya
{
	Vector3 Quaternion::TEMPVector30;
	Vector3 Quaternion::TEMPVector31;
	Vector3 Quaternion::TEMPVector32;
	Vector3 Quaternion::TEMPVector33;
	//Matrix3x3 Quaternion::_tempMatrix3x3();
	Quaternion Quaternion::DEFAULT;
	//TODO Quaternion Quaternion::NAN(NaN, NaN, NaN, NaN);
	void Quaternion::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up, Quaternion& out)
	{
		const Matrix3x3 _tempMatrix3x3;
		Matrix3x3::lookAt(eye, target, up, const_cast<Matrix3x3&>(_tempMatrix3x3));
		Quaternion::rotationMatrix(_tempMatrix3x3, out);
	}

	void Vector3::transformQuat(const Vector3& source, const Quaternion& rotation, Vector3& out)
	{
		float x = source.x, y = source.y, z = source.z, qx = rotation.x, qy = rotation.y, qz = rotation.z, qw = rotation.w,

			ix = qw * x + qy * z - qz * y, iy = qw * y + qz * x - qx * z, iz = qw * z + qx * y - qy * x, iw = -qx * x - qy * y - qz * z;

		out.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
		out.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
		out.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
	}
	void Quaternion::getYawPitchRoll(Vector3& out) const
	{

		Vector3::transformQuat(Vector3::_ForwardRH, *this, Quaternion::TEMPVector31/*forwarldRH*/);

		Vector3::transformQuat(Vector3::_Up, *this, Quaternion::TEMPVector32/*up*/);
		Vector3& upe = Quaternion::TEMPVector32;

		Quaternion::angleTo(Vector3::_ZERO, Quaternion::TEMPVector31, Quaternion::TEMPVector33/*angle*/);
		Vector3& angle = Quaternion::TEMPVector33;

		if (angle.x == PI / 2.0f)
		{
			angle.y = Quaternion::arcTanAngle(upe.z, upe.x);
			angle.z = 0.0f;
		}
		else if (angle.x == -PI / 2.0f)
		{
			angle.y = Quaternion::arcTanAngle(-upe.z, -upe.x);
			angle.z = 0.0f;
		}
		else
		{
			Matrix4x4 TEMPMatrix0;
			Matrix4x4 TEMPMatrix1;
			Matrix4x4::createRotationY(-angle.y, TEMPMatrix0);
			Matrix4x4::createRotationX(-angle.x, TEMPMatrix1);

			Vector3::transformCoordinate(Quaternion::TEMPVector32, TEMPMatrix0, Quaternion::TEMPVector32);
			Vector3::transformCoordinate(Quaternion::TEMPVector32, TEMPMatrix1, Quaternion::TEMPVector32);
			angle.z = Quaternion::arcTanAngle(upe.y, -upe.x);
		}

		// Special cases.
		if (angle.y <= -PI)
			angle.y = PI;
		if (angle.z <= -PI)
			angle.z = PI;

		if (angle.y >= PI && angle.z >= PI)
		{
			angle.y = 0.0f;
			angle.z = 0.0f;
			angle.x = PI - angle.x;
		}

		Vector3& oe = out;
		oe.x = angle.y;
		oe.y = angle.x;
		oe.z = angle.z;
	}
}

#include "util/Math.h"
#include "util/Debug.h"
#include <boost/functional/hash/hash.hpp>
#include <limits>
#include <cmath>

#define LODWORD(x) (*(DWORD*)(&x))

namespace RBX
{
	const G3D::Matrix3& Math::matrixRotateY()
	{
		static G3D::Matrix3 rotateY(0, 0, 1, 0, 1, 0, -1, 0, 0);
		return rotateY;
	}

	const G3D::Matrix3& Math::matrixTiltZ()
	{
		static G3D::Matrix3 tiltZ(0, 1, 0, -1, 0, 0, 0, 0, 1);
		return tiltZ;
	}

	bool Math::isDenormal(float f)
	{
		return (LODWORD(f) & 0x7F800000) == 0 && (LODWORD(f) & 0x7FFFFF) != 0;
	}

	bool Math::isNanInfDenorm(float f)
	{
		return f == std::numeric_limits<float>::infinity()
			|| f == -std::numeric_limits<float>::infinity()
			|| f == std::numeric_limits<float>::quiet_NaN()
			|| f == std::numeric_limits<float>::signaling_NaN()
			|| Math::isDenormal(f);
	}

	bool Math::isNanInfDenormVector3(const G3D::Vector3& v)
	{
		return Math::isNanInfDenorm(v.x)
			|| Math::isNanInfDenorm(v.y)
			|| Math::isNanInfDenorm(v.z);
	}

	bool Math::fixDenorm(G3D::Vector3& v)
	{
		bool fixed = false;

		float x = v.x;
		if (Math::isDenormal(x))
		{
			v.x = 0;
			fixed = true;
		}

		float y = v.y; 
		if (Math::isDenormal(y))
		{
			v.y = 0;
			fixed = true;
		}

		float z = v.z;
		if (Math::isDenormal(z))
		{
			v.z = 0;
			fixed = true;
		}

		return fixed;
	}

	float Math::radWrap(float rad)
	{
		if (rad >= -pi() && rad < pi())
			return rad;

		float rot = 1/twoPi();
		float f = floor((rad + pi()) * rot);
		return rad - Math::iRound(f) * twoPi();
	}

	G3D::Vector3 Math::sortVector3(const G3D::Vector3& v)
	{
		G3D::Vector3 result = v;

		if ( result.z < result.y)
		{
			std::swap(result.z, result.y);
		}

		if ( result.y < result.x  )
		{
			std::swap(result.y, result.x);
		}

		if ( result.z < result.y  )
		{
			std::swap(result.z, result.y);
		}

		return result;
	}

	G3D::Matrix3 Math::momentToObjectSpace(const G3D::Matrix3& iWorld, const G3D::Matrix3& bodyRotation)
	{
		return bodyRotation.transpose() * iWorld * bodyRotation;
	}

	G3D::Matrix3 Math::momentToWorldSpace(const G3D::Matrix3& iBody, const G3D::Matrix3& bodyRotation)
	{
		return (bodyRotation * iBody) * bodyRotation.transpose();
	}

	G3D::Vector3 Math::toDiagonal(const G3D::Matrix3& m)
	{
		return Vector3(m[0][0],m[1][1],m[2][2]);
	}

	G3D::Matrix3 Math::fromDiagonal(const G3D::Vector3& v)
	{
		return Matrix3(v.x, 0, 0, 0, v.y, 0, 0, 0, v.z);
	}

	bool Math::lessThan(const G3D::Vector3& min, const G3D::Vector3& max)
	{
		return min.x < max.x && min.y < max.y && min.z < max.z;
	}

	bool Math::isAxisAligned(const G3D::Matrix3& matrix)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				float f = matrix[i][j];
				if (f != 0 && f != 1 && f != -1)
					return false;
			}
		}

		return true;
	}

	G3D::Matrix3 Math::rotateAboutZ(const G3D::Matrix3& matrix, float radians)
	{
		Matrix3 m(Matrix3::identity());
		float sinR = sin(radians);
		float cosR = cos(radians);

		m.setColumn(0, Vector3(cosR, sinR, 0));
		m.setColumn(1, Vector3(-sinR, cosR, 0));

		return matrix * m;
	}

	void Math::rotateMatrixAboutY90(G3D::Matrix3& matrix, int times)
	{
		static G3D::Matrix3 y90(0, 0, 1, 0, 1, 0, -1, 0, 0);

		for (int i = 0; i < times; ++i)
		{
			matrix = y90 * matrix;
		}
	}

	float Math::getHeading(const G3D::Vector3& look)
	{
		return atan2(-look.x, -look.z);
	}

	float Math::getElevation(const G3D::Vector3& look)
	{
		return asin(look.y);
	}

	void Math::getHeadingElevation(const G3D::CoordinateFrame& c, float& heading, float& elevation)
	{
		Vector3 lookVector = c.getLookVector();

		heading = Math::getHeading(lookVector);
		elevation = Math::getElevation(lookVector);
	}

	void Math::setHeadingElevation(G3D::CoordinateFrame& c, float heading, float elevation)
	{
		float Y = sin(elevation);
		float unk = sqrtf(1 - Y * Y);
		float X = -(sin(heading) * unk);
		float Z = -(unk * cos(heading));
		c.lookAt(c.translation + Vector3(X, Y, Z).direction());
	}

	G3D::CoordinateFrame Math::getFocusSpace(const G3D::CoordinateFrame& focus)
	{
		Vector3 look = focus.lookVector();
		float heading = Math::getHeading(look);

		CoordinateFrame cf(focus.rotation);
		cf.translation = focus.translation;
		Math::setHeadingElevation(cf, heading, 0);
		return cf;
	}

	const G3D::Matrix3& Math::getAxisRotationMatrix(int face)
	{
		static Matrix3 y = Matrix3::fromEulerAnglesXYZ(0, 0, halfPi());
		static Matrix3 z = Matrix3::fromEulerAnglesXYZ(0, halfPi(), 0);
		static Matrix3 y_neg = Matrix3(y);
		static Matrix3 z_neg = Matrix3(z);

		switch (face)
		{
		case 1:
			return y;
		case 2:
			return z;
		case 4:
			return y_neg;
		case 5:
			return z_neg;
		default:
			return Matrix3::identity();
		}
	}

	G3D::Vector3 Math::vector3Abs(const G3D::Vector3& v)
	{
		return Vector3(fabs(v.x), fabs(v.y), fabs(v.z));
	}

	bool Math::isOrthonormal(const G3D::Matrix3& m)
	{
		return m.isOrthonormal();
	}

	bool Math::orthonormalizeIfNecessary(G3D::Matrix3& m)
	{
		if (!Math::isOrthonormal(m))
		{
			m.orthonormalize();
			return true;
		}

		return false;
	}

	size_t Math::hash(const G3D::Vector3& v)
	{
		const int magic = 1640531527;
		size_t h;

		h = (boost::hash_value(v.x) - magic);
		h = (boost::hash_value(v.y) - magic + (h >> 2) + (h << 6)) ^ h;
		h = (boost::hash_value(v.z) - magic + (h >> 2) + (h << 6)) ^ h;
		return h;
	}

	float Math::angle(const G3D::Vector3& v0, const G3D::Vector3& v1)
	{
		float f = v0.z * v1.z + v0.y * v1.y + v0.x * v1.x;
		if (f >= 1)
			return 0;
		if (f <= -1)
			return pi();
		return acos(f);
	}

	float Math::angleToE0(const G3D::Vector2& v)
	{
		Vector2 copy(v);
		copy.unitize();

		float f = acos(copy.x);
		if (copy.y < 0)
			return twoPi() - f;
		return f;
	}

	G3D::Vector3 Math::iRoundVector3(const G3D::Vector3& point)
	{
		return Vector3(Math::iRound(point.x), Math::iRound(point.y), Math::iRound(point.z));
	}

	const float& segSizeRadians()
	{
		// - 0.00001f is necessary to get the same value as the compiled binary
		static float ssr = (Math::pi() - 0.00001f) / 128;
		return ssr;
	}

	G3D::uint8 rotationToByteBase(float angle)
	{
		RBXAssert(angle <= Math::pi());
		RBXAssert(angle >= -Math::pi());

		float result = (angle + Math::pi()) / segSizeRadians();
		int resInt = G3D::iRound(result);
		
		RBXAssert(resInt >= -1);
		RBXAssert(resInt <= 256);

		resInt = std::max(0, resInt);
		resInt = std::min(255, resInt);

		G3D::uint8 byte = (G3D::uint8)resInt;

		RBXAssert(byte <= 255);

		return byte;
	}

	G3D::uint8 Math::rotationToByte(float angle)
	{
		return rotationToByteBase(angle);
	}

	float Math::rotationFromByte(unsigned char byteAngle)
	{
		float byteAngleFloat = byteAngle;
		return byteAngleFloat * segSizeRadians() - pi();
	}

	G3D::Vector3 Math::toGrid(const G3D::Vector3& v, const G3D::Vector3& grid)
	{
		Vector3 units = v / grid;
		return grid * Math::iRoundVector3(units);
	}

	G3D::Matrix3 Math::getIWorldAtPoint(const G3D::Vector3& cofmPos, const G3D::Vector3& worldPos, const G3D::Matrix3& iWorldAtCofm, float mass)
	{
		G3D::Vector3 delta = worldPos - cofmPos;
		float deltaXSquared = delta.x * delta.x;
		float deltaYSquared = delta.y * delta.y;
		float deltaZSquared = delta.z * delta.z;
		float deltaYX = delta.y * delta.x;
		float deltaZX = delta.z * delta.x;
		float deltaZY = delta.z * delta.y;

		return iWorldAtCofm + mass * Matrix3(
				deltaZSquared + deltaYSquared,
				-deltaYX,
				-deltaZX,
				-deltaYX,
				deltaZSquared + deltaXSquared,
				-deltaZY,
				-deltaZX,
				-deltaZY,
				deltaYSquared + deltaXSquared
				);
	}

	bool Math::intersectRayPlane(const G3D::Ray& ray, const G3D::Plane& plane, G3D::Vector3& hit)
	{
		float dotProd = ray.direction.dot(plane.normal());
		if (plane.halfSpaceContains(ray.origin))
		{
			if (!(dotProd < 0.0f))
			{
				hit = Vector3::inf();
				return false;
			}
		}
		else if (!(dotProd > 0.0f))
		{
			hit = Vector3::inf();
			return false;
		}
		G3D::Line myLine = Line::fromPointAndDirection(ray.origin, ray.direction);
		hit = myLine.intersection(plane);
		return true;
	}

	int Math::getOrientId(const G3D::Matrix3& matrix)
	{
		NormalId ColumnNormId = Vector3ToNormalId(matrix.getColumn(0));
		return Vector3ToNormalId(matrix.getColumn(1)) + 6 * ColumnNormId;
	}

	float Math::maxAxisLength(const G3D::Vector3& v)
	{
		G3D::Vector3 vAbs = Vector3(fabs(v.x), fabs(v.y), fabs(v.z));
		return std::max(vAbs.x, std::max(vAbs.y, vAbs.z));
	}
}

#include "util/Math.h"

using namespace RBX;

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

G3D::Matrix3 Math::momentToWorldSpace(const G3D::Matrix3& iBody, const G3D::Matrix3& bodyRotation)
{
	return (bodyRotation * iBody) * bodyRotation.transpose();
}

G3D::Vector3 Math::toDiagonal(const G3D::Matrix3& m)
{
	return Vector3(m[0][0],m[1][1],m[2][2]);
}
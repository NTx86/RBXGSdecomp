#include "Math.h"

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
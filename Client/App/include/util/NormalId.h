#pragma once
#include <g3d/vector3.h>
#include <g3d/matrix3.h>

namespace RBX
{
	enum NormalId
	{
		NORM_X = 0,
		NORM_Y = 1,
		NORM_Z = 2,
		NORM_X_NEG = 3,
		NORM_Y_NEG = 4,
		NORM_Z_NEG = 5,
		NORM_UNDEFINED = 6,
	};

	RBX::NormalId normalIdOpposite(RBX::NormalId normalId);
	bool validNormalId(NormalId normalId);
	NormalId Vector3ToNormalId(const G3D::Vector3& v);
	NormalId intToNormalId(int num);
	const G3D::Vector3& normalIdToVector3(NormalId normalId);
	const G3D::Matrix3& normalIdToMatrix3(NormalId normalId); // TODO: implement
	NormalId Matrix3ToNormalId(const G3D::Matrix3& m); // TODO: implement
}
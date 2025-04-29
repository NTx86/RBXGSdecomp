#include "util/NormalId.h"
#include "util/Debug.h"

using namespace G3D;

namespace RBX
{
	NormalId intToNormalId(int num)
	{
		return (NormalId)num;
	}

	bool validNormalId(NormalId id)
	{
		return id >= NORM_X && id <= NORM_Z_NEG;
	}

	NormalId RBX::normalIdOpposite(NormalId normalId)
	{
		return (NormalId)((normalId + NORM_X_NEG) % NORM_UNDEFINED);
	}

	//35% match, too lazy to do better right now
	const Vector3& normalIdToVector3(NormalId normalId)
	{
		switch (normalId)
		{
			case NORM_X:
				{
				static Vector3 x_0(1.0f, 0.0f, 0.0f);
				return x_0;
				}
			case NORM_Y:
				{
				static Vector3 y_0(0.0f, 1.0f, 0.0f);
				return y_0;
				}
			case NORM_Z:
				{
				static Vector3 z_0(0.0f, 0.0f, 1.0f);
				return z_0;
				}
			case NORM_X_NEG:
				{
				static Vector3 xn_0(-1.0f, 0.0f, 0.0f);
				return xn_0;
				}
			case NORM_Y_NEG:
				{
				static Vector3 yn_0(0.0f, -1.0f, 0.0f);
				return yn_0;
				}
			case NORM_Z_NEG:
				{
				static Vector3 zn_0(0.0f, 0.0f, -1.0f);
				return zn_0;
				}
		}
		RBXASSERT(0);
		return G3D::Vector3::zero();
	}

	NormalId Vector3ToNormalId(const G3D::Vector3& v)
	{
		//the assertion does not match, the rest of the code does
		RBXASSERT((v == Vector3::unitX()) || (v == Vector3::unitY()) || (v == Vector3::unitZ()) || (v == -Vector3::unitX()) || (v == -Vector3::unitY()) || (v == -Vector3::unitZ()));
		if (v.x == 1.0f)
			return NORM_X;
		if (v.y == 1.0f)
			return NORM_Y;
		if (v.z == 1.0f)
			return NORM_Z;
		if (v.x == -1.0f)
			return NORM_X_NEG;
		if (v.y == -1.0f)
			return NORM_Y_NEG;
		if (v.z == -1.0f)
			return NORM_Z_NEG;
		RBXASSERT(0);
		return NORM_UNDEFINED;
	}
}
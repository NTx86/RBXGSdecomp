#include "util/NormalId.h"
#include "util/Debug.h"

using namespace G3D;

namespace RBX
{
	bool validNormalId(NormalId id)
	{
		return id >= NORM_X && id <= NORM_Z_NEG;
	}

	NormalId Vector3ToNormalId(const G3D::Vector3& v)
	{
		//the assertion does not match, the rest of the code does
		RBXAssert((v == Vector3::unitX()) || (v == Vector3::unitY()) || (v == Vector3::unitZ()) || (v == -Vector3::unitX()) || (v == -Vector3::unitY()) || (v == -Vector3::unitZ()));
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
		RBXAssert(0);
		return NORM_UNDEFINED;
	}
}
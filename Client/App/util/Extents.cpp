#include "util/Extents.h"
#include "util/Math.h"

namespace RBX
{
	float Extents::areaXZ() const
	{
		Vector3 delta = this->high - this->low;
		return delta.x * delta.z;
	}
}

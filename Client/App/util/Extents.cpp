#include "util/Extents.h"
#include "util/Math.h"
#include "util/Debug.h"

namespace RBX
{
	float Extents::areaXZ() const
	{
		Vector3 delta = this->high - this->low;
		return delta.x * delta.z;
	}

	Vector3 Extents::getCorner(int i) const
	{
		RBXAssert(i >= 0);
		RBXAssert(i <= 7);

		return Vector3(
			*(&this->low.x + 3 * (i / 4)),
			*(&this->low.y + 3 * (i / 2 % 2)),
			*(&this->low.z + 3 * (i % 2))
		);
	}
}

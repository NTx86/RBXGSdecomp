#include "v8world/SpatialHash.h"
#include "util/debug.h"

namespace RBX
{
	int SpatialHash::getHash(const Vector3int32& grid)
	{
		int result = grid.x * -0xba3 ^ grid.y * 0x409f ^ grid.z * -0x49;
		result &= 0xffff;
		RBXASSERT(result >= 0);
		RBXASSERT(result < 0x10000);
		return result;
	}
}
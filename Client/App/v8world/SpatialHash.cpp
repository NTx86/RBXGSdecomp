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

	Vector3int32 SpatialHash::realToHashGrid(const Vector3& realPoint)
	{
		return Vector3int32::floor(realPoint * 0.125f);
	}

	Extents SpatialHash::hashGridToRealExtents(const G3D::Vector3& hashGrid)
	{
		return Extents(hashGrid * 8.0f, (hashGrid + Vector3(1, 1, 1)) * 8.0f);
	}

	void SpatialHash::computeMinMax(const Extents& extents, Vector3int32& min, Vector3int32& max)
	{
		min = SpatialHash::realToHashGrid(extents.min());
		max = SpatialHash::realToHashGrid(extents.max());
	}

	void SpatialHash::removeNodeFromHash(SpatialNode* remove)
	{
		SpatialNode** temp = &this->nodes[remove->hashId];
		while (*temp != remove)
		{
			temp = &(*temp)->nextHashLink;
		}
		*temp = remove->nextHashLink;
	}

	SpatialNode* SpatialHash::findNode(Primitive* p, const Vector3int32& grid)
	{
		int hash = SpatialHash::getHash(grid);
		SpatialNode* result = this->nodes[hash];
		while (result->primitive != p || result->gridId != grid)
		{
			result = result->nextHashLink;
		}
		return result;
	}
}
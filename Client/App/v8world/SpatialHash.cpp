#include "v8world/SpatialHash.h"
#include "v8world/Primitive.h"
#include "v8world/ContactManager.h"
#include "util/debug.h"

namespace RBX
{
	size_t SpatialHash::numBuckets()
	{
		return 0x10000;
	}

	int SpatialHash::getHash(const Vector3int32& grid)
	{
		int result = grid.x * -0xba3 ^ grid.y * 0x409f ^ grid.z * -0x49;
		result &= (numBuckets()-1);
		RBXASSERT(result >= 0);
		RBXASSERT(result < (int)numBuckets());
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

	void SpatialHash::destroyNode(SpatialNode* destroy)
	{
		SpatialNode* nextPrimitiveLink = destroy->nextPrimitiveLink;
		SpatialNode* prevPrimitiveLink = destroy->prevPrimitiveLink;

		if (nextPrimitiveLink)
			nextPrimitiveLink->prevPrimitiveLink = prevPrimitiveLink;

		if (prevPrimitiveLink)
			prevPrimitiveLink->nextPrimitiveLink = nextPrimitiveLink;
		else
			destroy->primitive->spatialNodes = nextPrimitiveLink;

		removeNodeFromHash(destroy);

		int destroyHash = destroy->hashId;
		Vector3int32 destroyGrid = destroy->gridId;

		for (SpatialNode* node = nodes[destroyHash]; node != NULL; node = node->nextHashLink)
		{
			if (node->gridId == destroyGrid)
			{
				Primitive* destroyPrim = destroy->primitive;
				Primitive* nodePrim = node->primitive;
				RBXASSERT(nodePrim != destroyPrim);
				if (Primitive::getContact(destroyPrim, nodePrim) && !shareCommonGrid(destroyPrim, nodePrim))
					this->contactManager->onReleasePair(destroyPrim, nodePrim);
			}
		}

		destroy->nextHashLink = this->extraNodes;
		--this->nodesOut;
		this->extraNodes = destroy;
	}

	bool SpatialHash::shareCommonGrid(Primitive* me, Primitive* other)
	{
		SpatialNode* spatialNodes = me->spatialNodes;
		while (true)
		{
			if (!spatialNodes)
				return false;
			int hashId = spatialNodes->hashId;
			SpatialNode* currentNode = this->nodes[hashId];
			while (currentNode)
			{
				if (currentNode->primitive == other && currentNode->gridId == spatialNodes->gridId)
					return true;
				currentNode = currentNode->nextHashLink;
			}
			spatialNodes = spatialNodes->nextPrimitiveLink;
		}
	}
}
#pragma once
#include <vector>
#include "util/Extents.h"
#include "util/Vector3int32.h"

namespace RBX
{
	class Primitive;
	class World;
	class ContactManager;

	class SpatialNode
	{
	public:
		Primitive* primitive;
		SpatialNode* nextHashLink;
		SpatialNode* nextPrimitiveLink;
		SpatialNode* prevPrimitiveLink;
		int hashId;
		Vector3int32 gridId;

	public:
		SpatialNode();
	};

	class SpatialHash
	{
	private:
		World* world;
		ContactManager* contactManager;
		std::vector<SpatialNode*> nodes;
		SpatialNode* extraNodes;
		int nodesOut;
		int maxBucket;
	  
	private:
		SpatialNode* newNode();
		void returnNode(SpatialNode*);
		bool shareCommonGrid(Primitive*, Primitive*);
		bool hashHasPrimitive(Primitive*, int, const Vector3int32&);
		SpatialNode* findNode(Primitive*, const Vector3int32&);
		void removeNodeFromHash(SpatialNode*);
		void insertNodeToPrimitive(SpatialNode*, Primitive*, const Vector3int32&, int);
		void removeNodeFromPrimitive(SpatialNode*);
		void addNode(Primitive*, const Vector3int32&);
		void destroyNode(SpatialNode*);
		void changeMinMax(Primitive*, const Extents&, const Extents&, const Extents&);
		void primitiveExtentsChanged(Primitive*);
		unsigned int numNodes(unsigned int) const;
	public:
		//SpatialHash(const SpatialHash&);
		SpatialHash(World*, ContactManager*);
		~SpatialHash();
	public:
		void onPrimitiveAdded(Primitive*);
		void onPrimitiveRemoved(Primitive*);
		void onPrimitiveExtentsChanged(Primitive*);
		void onAllPrimitivesMoved();
		void getPrimitivesInGrid(const Vector3int32&, G3D::Array<Primitive*>&);
		bool getNextGrid(Vector3int32&, const G3D::Ray&, float);
		void getPrimitivesTouchingExtents(const Extents&, const Primitive*, G3D::Array<Primitive*>&);
		int getNodesOut() const;
		int getMaxBucket() const;
		void doStats() const;
		//SpatialHash& operator=(const SpatialHash&);
	  
	private:
		static float hashGridSize();
		static float hashGridRecip();
		static unsigned int numBuckets();
		static unsigned int numBits();
		static int getHash(const Vector3int32& grid);
		static Extents computeMinMax(const Extents&);
		static void computeMinMax(const Extents&, Vector3int32&, Vector3int32&);
		static void computeMinMax(const Primitive*, Vector3int32&, Vector3int32&);
	public:
		static Vector3int32 realToHashGrid(const G3D::Vector3& realPoint);
		static G3D::Vector3 hashGridToReal(const G3D::Vector3&);
		static Extents hashGridToRealExtents(const G3D::Vector3&);
	};
}

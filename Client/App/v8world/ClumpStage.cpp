#include "v8world/ClumpStage.h"
#include "v8world/Clump.h"
#include "v8world/Primitive.h"
#include "v8world/RigidJoint.h"
#include "v8world/MotorJoint.h"
#include "v8world/CollisionStage.h"
#include "v8world/AssemblyStage.h"
#include "v8world/Assembly.h"
#include "v8world/Anchor.h"

namespace RBX
{
	// TODO: check which of these belong in the headers

	PrimitiveSort::PrimitiveSort()
		: anchored(false),
		  surfaceAreaJoints(0)
	{
	}

	PrimitiveEntry::PrimitiveEntry(Primitive* primitive, PrimitiveSort power)
		: primitive(primitive),
		  power(power)
	{
	}

	AnchorEntry::AnchorEntry(Anchor* anchor, int size)
		: anchor(anchor),
		  size(size)
	{
	}

	RigidEntry::RigidEntry(RigidJoint* rigidJoint, PrimitiveSort power)
		: rigidJoint(rigidJoint),
		  power(power)
	{
	}

	PrimitiveSort ClumpStage::getRigidPower(RigidJoint* r)
	{
		Clump* clump0 = r->getPrimitive(0)->getClump();
		Clump* clump1 = r->getPrimitive(1)->getClump();
		RBXAssert((clump0 == NULL) != (clump1 == NULL));
		if (!clump0)
			clump0 = clump1;
		RBXAssert(clump0);
		return PrimitiveSort(clump0->getRootPrimitive());
	}

	int ClumpStage::numClumps(RigidJoint* r)
	{
		int count = 0;
		if (r->getPrimitive(0)->getClump())
			++count;
		if (r->getPrimitive(1)->getClump())
			++count;
		return count;
	}

	PrimitiveSort ClumpStage::getMotorPower(const MotorJoint* m)
	{
		Clump* clump0 = m->getPrimitive(0)->getClump();
		Clump* clump1 = m->getPrimitive(1)->getClump();
		RBXAssert(clump0 && clump1);

		if (clump0 == clump1)
		{
			return PrimitiveSort();
		}
		else
		{
			Primitive* p1 = clump1->getRootPrimitive();
			Primitive* p0 = clump0->getRootPrimitive();
			PrimitiveSort ps1 = PrimitiveSort(p1);
			PrimitiveSort ps0 = PrimitiveSort(p0);

			if (ps1 < ps0)
				return ps0;
			else
				return ps1;
		}
	}

	void ClumpStage::onPrimitiveCanCollideChanged(Primitive* p)
	{
		IStage* collisionIStage = findStage(COLLISION_STAGE);
		rbx_static_cast<CollisionStage*>(collisionIStage)->onPrimitiveCanCollideChanged(p);
	}

	int ClumpStage::getMetric(MetricType metricType)
	{
		if (metricType != MAX_TREE_DEPTH)
			return IWorldStage::getMetric(metricType);

		return 0;
	}

	bool ClumpStage::upToDate()
	{
		return 
			anchors.empty() &&
			rigidTwos.empty() &&
			rigidOnes.empty() &&
			rigidZeros.empty() &&
			primitives.empty() &&
			motors.empty() &&
			anchoredClumps.empty() &&
			freeClumps.empty() &&
			assemblies.empty();
	}

	void ClumpStage::stepUi(int uiStepId)
	{
		RBXAssert(upToDate());
		rbx_static_cast<AssemblyStage*>(getDownstreamWS())->stepUi(uiStepId);
	}

	bool ClumpStage::motorsFind(MotorJoint* m)
	{
		return std::find(motors.begin(), motors.end(), m) != motors.end();
	}

	__forceinline bool ClumpStage::edgesFind(Edge* e)
	{
		return edges.find(e) != edges.end();
	}

	void ClumpStage::motorsErase(MotorJoint* m)
	{
		RBXAssert(motorsFind(m));
		motors.erase(std::find(motors.begin(), motors.end(), m));
	}

	void ClumpStage::motorAnglesErase(MotorJoint* m)
	{
		size_t removed = motorAngles.erase(m);
		RBXAssert(removed == 1);
	}

	void ClumpStage::processEdges()
	{
		while (!edges.empty())
		{
			Edge* e = *edges.begin();
			edges.erase(edges.begin());

			RBXAssert(e->inStage(this));

			Assembly* a0 = e->getPrimitive(0)->getAssembly();
			Assembly* a1 = e->getPrimitive(1)->getAssembly();

			RBXAssert(a0 && a1);
			RBXAssert(a0->downstreamOfStage(this) && a1->downstreamOfStage(this));

			if (a0 == a1)
			{
				a0->addInternalEdge(e);
			}
			else
			{
				a0->addExternalEdge(e);
				a1->addExternalEdge(e);

				if (!a0->getAnchored() || !a1->getAnchored())
				{
					getDownstreamWS()->onEdgeAdded(e);
				}
			}
		}
	}

	void ClumpStage::updateMotorJoint(MotorJoint* m)
	{
		RBXAssert(m->getPrimitive(0)->getAssembly());
		RBXAssert(m->getPrimitive(1)->getAssembly());
		RBXAssert(!motorsFind(m));

		Primitive* p0 = m->getPrimitive(0);
		Primitive* p1 = !p0->getClump()->getRootPrimitive()->getBody()->getParent() ? m->getPrimitive(1) : p0;
		if (p1 == p0)
			p0 = m->getPrimitive(1);

		Primitive* p1Root = p1->getClump()->getRootPrimitive();
		RBXAssert(p1Root->getBody()->getParent() == p0->getBody());

		G3D::CoordinateFrame mChildInMParent = m->getMeInOther(p1);
		if (p1 == p1Root)
		{
			p1Root->getBody()->setMeInParent(mChildInMParent);
		}
		else
		{
			G3D::CoordinateFrame mChildInCRoot = p1->getBody()->getMeInDescendant(p1Root->getBody());
			G3D::CoordinateFrame cRootInMChild = mChildInCRoot.inverse();
			G3D::CoordinateFrame cRootInMParent = mChildInMParent * cRootInMChild;
			p1Root->getBody()->setMeInParent(cRootInMParent);
		}
	}

	__forceinline float calculateAnchorSize2(const Vector3& size)
	{
		if (size.x >= size.y)
		{
			if (size.y < size.z)
				return size.x * size.z;
			else
				return size.x * size.y;
		}
		else if (size.x < size.z)
		{
			return size.y * size.z;
		}
		else
		{
			return size.x * size.y;
		}
	}

	__forceinline int calculateAnchorSize(const Vector3& size)
	{
		return G3D::iRound(floor(calculateAnchorSize2(size)));
	}

	void ClumpStage::anchorsInsert(Anchor* a)
	{
		int size = calculateAnchorSize(a->getPrimitive()->getGridSize());

		bool inserted;
		inserted = anchorSizeMap.insert(std::pair<Anchor*, int>(a, size)).second;
		RBXAssert(inserted);

		inserted = anchors.insert(AnchorEntry(a, size)).second;
		RBXAssert(inserted);
	}

	void ClumpStage::rigidTwosInsert(RigidJoint* r)
	{
		RBXAssert(!r->getPrimitive(0)->getClump() || !r->getPrimitive(0)->getClump()->containsInconsistent(r));
		RBXAssert(!r->getPrimitive(1)->getClump() || !r->getPrimitive(1)->getClump()->containsInconsistent(r));
	
		bool inserted = rigidTwos.insert(r).second;
		RBXAssert(inserted);
	}

	void ClumpStage::rigidOnesInsert(RigidJoint* r)
	{
		PrimitiveSort sort;
		{
			// WTF? why does this result in a better match???
			PrimitiveSort sortTemp = getRigidPower(r);
			sort = sortTemp;
		}

		bool inserted;
		inserted = rigidJointPowerMap.insert(std::pair<RigidJoint*, PrimitiveSort>(r, sort)).second;
		RBXAssert(inserted);

		inserted = rigidOnes.insert(RigidEntry(r, sort)).second;
		RBXAssert(inserted);
	}

	void ClumpStage::rigidZerosInsert(RigidJoint* r)
	{
		bool inserted = rigidZeros.insert(r).second;
		RBXAssert(inserted);
	}

	void ClumpStage::primitivesInsert(Primitive* p)
	{
		//PrimitiveSort sort(p);
		PrimitiveSort sort;
		{
			// WTF? why does this result in a better match???
			PrimitiveSort sortTemp(p);
			sort = sortTemp;
		}

		bool inserted;
		inserted = primitiveSizeMap.insert(std::pair<Primitive*, PrimitiveSort>(p, sort)).second;
		RBXAssert(inserted);

		inserted = primitives.insert(PrimitiveEntry(p, sort)).second;
		RBXAssert(inserted);
	}

	bool ClumpStage::rigidTwosFind(RigidJoint* r)
	{
		return rigidTwos.find(r) != rigidTwos.end();
	}

	bool ClumpStage::rigidOnesFind(RigidJoint* r)
	{
		return rigidJointPowerMap.find(r) != rigidJointPowerMap.end();
	}

	bool ClumpStage::rigidZerosFind(RigidJoint* r)
	{
		return rigidZeros.find(r) != rigidZeros.end();
	}

	bool ClumpStage::motorAnglesFind(MotorJoint* m)
	{
		return motorAngles.find(m) != motorAngles.end();
	}

	void ClumpStage::motorsInsert(MotorJoint* m)
	{
		RBXAssert(!motorsFind(m));
		motors.push_back(m);
	}

	void ClumpStage::motorAnglesInsert(MotorJoint* m)
	{
		bool inserted = motorAngles.insert(m).second;
		RBXAssert(inserted);
	}

	void ClumpStage::edgesInsert(Edge* e)
	{
		bool inserted = edges.insert(e).second;
		RBXAssert(inserted);
	}

	void ClumpStage::addEdge(Edge* e)
	{
		edgesInsert(e);
	}

	void ClumpStage::removeEdge(Edge* e)
	{
		Assembly* a0 = e->getPrimitive(0)->getAssembly();
		Assembly* a1 = e->getPrimitive(1)->getAssembly();

		if (e->downstreamOfStage(this))
		{
			getDownstreamWS()->onEdgeRemoving(e);

			RBXAssert(a0 && a1 && a0 != a1);
			a0->removeExternalEdge(e);
			a1->removeExternalEdge(e);

			RBXAssert(edges.find(e) == edges.end());
		}
		else
		{
			size_t removed = edges.erase(e);
			if (removed != 1)
			{
				RBXAssert(a0 && a1);
				if (a0 == a1)
				{
					a0->removeInternalEdge(e);
				}
				else
				{
					RBXAssert(a0->getAnchored() && a1->getAnchored());

					a0->removeExternalEdge(e);
					a1->removeExternalEdge(e);
				}
			}
		}

		RBXAssert(e->inStage(this));
	}

	// 100% match if downstreamOfStage in RBXAssert has __forceinline
	void ClumpStage::removeExternalEdge(Edge* e)
	{
		Assembly* a0 = e->getPrimitive(0)->getAssembly();
		Assembly* a1 = e->getPrimitive(1)->getAssembly();

		RBXAssert(e->downstreamOfStage(this) != (a0->getAnchored() && a1->getAnchored()));
		RBXAssert(!edgesFind(e));

		a0->removeExternalEdge(e);
		a1->removeExternalEdge(e);
		if (e->downstreamOfStage(this))
			getDownstreamWS()->onEdgeRemoving(e);

		edgesInsert(e);
	}

	void ClumpStage::removeInternalEdge(Edge* e)
	{
		RBXAssert(e->inStage(this));
		RBXAssert(!edgesFind(e));
		RBXAssert(e->getPrimitive(0)->getAssembly() == e->getPrimitive(1)->getAssembly());

		e->getPrimitive(0)->getAssembly()->removeInternalEdge(e);
		edgesInsert(e);
	}

	void ClumpStage::removeAssemblyEdges(Assembly* a)
	{
		while (!a->getExternalEdges().empty())
		{
			removeExternalEdge(*a->getExternalEdges().begin());
		}

		while (!a->getInternalEdges().empty())
		{
			removeInternalEdge(*a->getInternalEdges().begin());
		}
	}

	void ClumpStage::onPrimitiveAdded(Primitive* p)
	{
		RBXAssert(!p->getFirstEdge());
		p->putInStage(this);
		primitivesInsert(p);
		if (p->getAnchorObject())
			anchorsInsert(p->getAnchorObject());
	}

	void ClumpStage::onPrimitiveAddedAnchor(Primitive* p)
	{
		anchorsInsert(p->getAnchorObject());
	}

	void ClumpStage::onMotorAngleChanged(MotorJoint* m)
	{
		RBXAssert(motorsFind(m) || (m->getPrimitive(0)->getAssembly() && m->getPrimitive(1)->getAssembly()));

		if (!motorAnglesFind(m))
			motorAnglesInsert(m);
	}

	bool ClumpStage::inBuffers(RigidJoint* r)
	{
		return rigidTwosFind(r) || rigidZerosFind(r) || rigidOnesFind(r);
	}

	void ClumpStage::anchorsErase(Anchor* a)
	{
		typedef std::map<Anchor*, int>::iterator Iterator;

		Iterator it = anchorSizeMap.find(a);

		std::pair<Anchor*, int> pair = *it;
		AnchorEntry entry(a, pair.second); 

		size_t removed = anchors.erase(entry);
		RBXAssert(removed == 1);

		anchorSizeMap.erase(it);
	}

	void ClumpStage::rigidZerosErase(RigidJoint* r)
	{
		size_t removed = rigidZeros.erase(r);
		RBXAssert(removed == 1);
	}

	void ClumpStage::rigidOnesErase(RigidJoint* r)
	{
		typedef std::map<RigidJoint*, PrimitiveSort>::iterator Iterator;

		Iterator it = rigidJointPowerMap.find(r);

		std::pair<RigidJoint*, PrimitiveSort> pair = *it;
		RigidEntry entry(r, pair.second); 

		size_t removed = rigidOnes.erase(entry);
		RBXAssert(removed == 1);

		rigidJointPowerMap.erase(it);
	}

	void ClumpStage::primitivesErase(Primitive* p)
	{
		typedef std::map<Primitive*, PrimitiveSort>::iterator Iterator;

		Iterator it = primitiveSizeMap.find(p);

		std::pair<Primitive*, PrimitiveSort> pair = *it;
		PrimitiveEntry entry(p, pair.second); 

		size_t removed = primitives.erase(entry);
		RBXAssert(removed == 1);

		primitiveSizeMap.erase(it);
	}

	ClumpStage::~ClumpStage()
	{
	}

	// impossible to match: cant get the specific std::set functions at the rigidjoint section to inline
	bool ClumpStage::processPrimitives()
	{
		typedef std::set<PrimitiveEntry, PrimitiveSortCriterion>::const_iterator Iterator;

		while (!primitives.empty())
		{
			Iterator it = primitives.begin();
			it--;

			Primitive* p = (*it).primitive;
			primitivesErase(p);

			RBXAssert(!p->getClump());
			Clump* c = new Clump(p);
			freeClumps.insert(c);

			RigidJoint* r = p->getFirstRigid();
			if (r)
			{
				for (; r != NULL; r = p->getNextRigid(r))
				{
					rigidZerosErase(r);
					rigidOnesInsert(r);
				}

				return false;
			}
		}

		RBXAssert(rigidZeros.empty());
		return true;
	}

	void ClumpStage::processAssemblies()
	{
		while (!assemblies.empty())
		{
			Assembly* a = *assemblies.begin();
			size_t removed = assemblies.erase(a);
			RBXAssert(removed == 1);
			a->putInPipeline(this);
			rbx_static_cast<AssemblyStage*>(getDownstreamWS())->onAssemblyAdded(a);
		}
	}

	void ClumpStage::processMotorAngles()
	{
		while (!motorAngles.empty())
		{
			MotorJoint* j = *motorAngles.begin();
			updateMotorJoint(j);
			size_t removed = motorAngles.erase(j);
			RBXAssert(removed == 1);
		}
	}

	bool ClumpStage::removeFromBuffers(RigidJoint* r)
	{
		if (rigidTwos.erase(r) != 0 || rigidZeros.erase(r) != 0)
			return true;

		if (rigidOnesFind(r))
		{
			rigidOnesErase(r);
			return true;
		}

		return false;
	}

#pragma warning (push)
#pragma warning (disable : 4355) // warning C4355: 'this' : used in base member initializer list
	ClumpStage::ClumpStage(IStage* upstream, World* world)
		: IWorldStage(upstream, new AssemblyStage(this, world), world)
	{
	}
#pragma warning (pop)

	// 71% match if the numClumps inside of the RBXAssert has __declspec(noinline)
	void ClumpStage::destroyClumpGuts(Clump* c)
	{
		typedef std::set<Primitive*>::const_iterator PrimIterator;

		for (PrimIterator it = c->clumpPrimBegin(); it != c->clumpPrimEnd(); it++)
		{
			Primitive* p = *it;
			primitivesInsert(p);
		}

		if (c->getAnchored())
		{
			Anchor* anchor = c->removeAnchor();
			anchorsInsert(anchor);
		}

		while (!c->getInconsistents().empty())
		{
			RigidJoint* r = *c->getInconsistents().begin();

			Clump* otherClump = c->otherClump(r);
			RBXAssert(otherClump == c);

			otherClump->removeInconsistent(r);
			c->removeInconsistent(r);
			rigidTwosInsert(r);
		}

		std::set<RigidJoint*> internalRs;
		std::set<RigidJoint*> externalRs;
		c->getRigidJoints(internalRs, externalRs);

		typedef std::set<RigidJoint*>::const_iterator RigidIterator;

		for (RigidIterator it = internalRs.begin(); it != internalRs.end(); it++)
		{
			RigidJoint* r = *it;
			RBXAssert(r->getPrimitive(0)->getClump() == r->getPrimitive(1)->getClump());
			if (!inBuffers(r))
				rigidTwosInsert(r);
		}

		c->removeAllPrimitives();

		for (RigidIterator it = externalRs.begin(); it != externalRs.end(); it++)
		{
			RigidJoint* r = *it;

			size_t removed = rigidTwos.erase(r);
			if (removed == 0)
			{
				removed = rigidZeros.erase(r);
				if (removed == 0)
				{
					if (rigidOnesFind(r))
						rigidOnesErase(r);
				}
			}

			RBXAssert(numClumps(r) < 2);
			int count = numClumps(r);
			if (count != 1)
				rigidOnesInsert(r);
			else
				rigidZerosInsert(r);
		}
	}

	void ClumpStage::addMotor(MotorJoint* m)
	{
		motorsInsert(m);
		motorAnglesInsert(m);
	}

	// 100% match if inOrDownstreamOfStage has __forceinline
	void ClumpStage::onEdgeAdded(Edge* e)
	{
		RBXAssert(e->getPrimitive(0)->inOrDownstreamOfStage(this));
		RBXAssert(e->getPrimitive(1)->inOrDownstreamOfStage(this));

		Primitive::insertEdge(e);
		e->putInStage(this);
		if (RigidJoint::isRigidJoint(e))
		{
			rigidTwosInsert(rbx_static_cast<RigidJoint*>(e));
		}
		else if (MotorJoint::isMotorJoint(e))
		{
			addMotor(rbx_static_cast<MotorJoint*>(e));
		}
		else
		{
			addEdge(e);
		}
	}

	// 82% match if MotorJoint::isMotorJoint and RigidJoint::isRigidJoint has __forceinline
	// MotorJoint::isMotorJoint and RigidJoint::isRigidJoint aren't matching 100%, preventing this from being matched
	void ClumpStage::removeFromAssemblyFast(Primitive* p)
	{
		Assembly* a = p->getAssembly();
		RBXAssert(a);

		Edge* e = p->getFirstEdge();

		if (e)
		{
			for (; e != NULL; e = p->getNextEdge(e))
			{
				if (e->getEdgeType() == Edge::JOINT)
				{
					if (MotorJoint::isMotorJoint(e))
						break;
				}

				if (e->getEdgeType() == Edge::CONTACT || RigidJoint::isRigidJoint(e))
				{
					if (a->containsExternalEdge(e))
						removeExternalEdge(e);
					else if (a->containsInternalEdge(e))
						removeInternalEdge(e);
				}
			}

			destroyAssembly(p->getAssembly());
		}
	}

	// 100% match if removeFromBuffers and rigidOnesFind have __forceinline
	void ClumpStage::removeFromClumpFast(Primitive* p, RigidJoint* toParent)
	{
		if (p->getAssembly())
			removeFromAssemblyFast(p);

		Clump* c = p->getClump();

		for (RigidJoint* r = p->getFirstRigid(); r != NULL; r = p->getNextRigid(r))
		{
			if (c->containsInconsistent(r))
			{
				RBXAssert(!inBuffers(r));
				c->removeInconsistent(r);
				c->otherClump(r)->removeInconsistent(r);
			}
			else if (inBuffers(r))
			{
				removeFromBuffers(r);
			}

			rigidTwosInsert(r);
		}

		primitivesInsert(p);
		c->removePrimitive(p);
	}

	void ClumpStage::removeSpanningTreeFast(Primitive* topPrim, RigidJoint* toParent)
	{
		for (RigidJoint* r = topPrim->getFirstRigid(); r != NULL; r = topPrim->getNextRigid(r))
		{
			if (SpanLink::isDownstreamSpanningJoint(topPrim, r))
			{
				removeSpanningTreeFast(r->otherPrimitive(topPrim), r);
			}
		}

		removeFromClumpFast(topPrim, toParent);
	}

	void ClumpStage::removeMotor(MotorJoint* m)
	{
		if (!motorsFind(m))
		{
			Assembly* a = m->getPrimitive(0)->getAssembly();
			RBXAssert(a == m->getPrimitive(1)->getAssembly());
			destroyAssembly(a);
		}

		motorsErase(m);
		if (motorAnglesFind(m))
			motorAnglesErase(m);
	}
}

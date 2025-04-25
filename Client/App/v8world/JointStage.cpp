#include "v8world/JointStage.h"
#include "v8world/Edge.h"
#include "v8world/ClumpStage.h"
#include "v8world/Primitive.h"
#include "v8world/Joint.h"

namespace RBX
{
	// TODO: determine which of these functions need to go into the header

	// TODO: move to header
	__forceinline bool JointStage::edgeHasPrimitivesDownstream(Edge* e)
	{
		Primitive* p0 = e->getPrimitive(0);
		Primitive* p1 = e->getPrimitive(1);
		return
			p0 &&
			p1 &&
			p0->inPipeline() &&
			p1->inPipeline() &&
			p0->downstreamOfStage(this) &&
			p1->downstreamOfStage(this);
	}

	void JointStage::moveEdgeToDownstream(Edge* e)
	{
		RBXAssert(edgeHasPrimitivesDownstream(e));

		IStage* downstream = getDownstream();
		ClumpStage* clumpStage = rbx_static_cast<ClumpStage*>(downstream);
		clumpStage->onEdgeAdded(e);
	}

	void JointStage::removeEdgeFromDownstream(Edge* e)
	{
		RBXAssert(edgeHasPrimitivesDownstream(e));

		IStage* downstream = getDownstream();
		ClumpStage* clumpStage = rbx_static_cast<ClumpStage*>(downstream);
		clumpStage->onEdgeRemoving(e);
	}

	bool JointStage::pairInMap(Joint* j, Primitive* p)
	{
		typedef std::multimap<Primitive*, Joint*>::const_iterator Iterator;
		Iterator iter = jointMap.lower_bound(p);
		Iterator iter2 = jointMap.upper_bound(p);

		for (; iter != iter2; iter++)
		{
			if (iter->second == j)
				return true;
		}

		return false;
	}

	void JointStage::insertToMap(Joint* j, Primitive* p)
	{
		if (!p)
			return;

		RBXAssert(!pairInMap(j, p));
		jointMap.insert(std::pair<Primitive*, Joint*>(p, j));
	}

	__declspec(noinline) void JointStage::removeFromMap(Joint* j, Primitive* p)
	{
		// TODO
		// remove noinline once completed
		RBXAssert(0);
	}

	// NOTE: might be in headers
	bool JointStage::jointInList(Joint* j)
	{
		return incompleteJoints.find(j) != incompleteJoints.end();
	}

	void JointStage::removeFromList(Joint* j)
	{
		size_t count = incompleteJoints.erase(j);
		RBXAssert(count == 1);
	}

	void JointStage::insertToList(Joint* j)
	{
		std::pair<std::set<Joint*>::iterator, bool> result = incompleteJoints.insert(j);
		RBXAssert(result.second);
	}

	void JointStage::moveJointToDownstream(Joint* j)
	{
		RBXAssert(!jointInList(j));
		RBXAssert(!pairInMap(j, j->getPrimitive(0)));
		RBXAssert(!pairInMap(j, j->getPrimitive(1)));

		moveEdgeToDownstream(j);
	}

	void JointStage::removeJointFromDownstream(Joint* j)
	{
		RBXAssert(!jointInList(j));
		RBXAssert(!pairInMap(j, j->getPrimitive(0)));
		RBXAssert(!pairInMap(j, j->getPrimitive(1)));

		removeEdgeFromDownstream(j);
	}

	void JointStage::onJointPrimitiveNulling(Joint* j, Primitive* nulling)
	{
		RBXAssert(!nulling);
		RBXAssert(!j->links(nulling));
		RBXAssert(j->downstreamOfStage(this) == edgeHasPrimitivesDownstream(j));
		RBXAssert(j->downstreamOfStage(this) != edgeHasPrimitivesDownstream(j));

		if (j->downstreamOfStage(this))
		{
			RBXAssert(!pairInMap(j, nulling));
			removeJointFromDownstream(j);
			insertToList(j);
			insertToMap(j, j->otherPrimitive(nulling));
		}
		else
		{
			RBXAssert(!j->inStage(this));
			removeFromMap(j, nulling);
		}

		RBXAssert(!pairInMap(j, nulling));
	}

	void JointStage::onJointPrimitiveSet(Joint* j, Primitive* p)
	{
		RBXAssert(!p);
		RBXAssert(!j->links(p));
		RBXAssert(j->inStage(this));
		RBXAssert(!jointInList(j));

		if (edgeHasPrimitivesDownstream(j))
		{
			RBXAssert(!pairInMap(j, p));
			RBXAssert(pairInMap(j, j->otherPrimitive(p)));

			removeFromList(j);
			removeFromMap(j, j->otherPrimitive(p));
			moveJointToDownstream(j);
		}
		else
		{
			insertToMap(j, p);
		}
	}
}
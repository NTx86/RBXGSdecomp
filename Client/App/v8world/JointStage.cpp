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
		RBXASSERT(edgeHasPrimitivesDownstream(e));

		IStage* downstream = getDownstream();
		ClumpStage* clumpStage = rbx_static_cast<ClumpStage*>(downstream);
		clumpStage->onEdgeAdded(e);
	}

	void JointStage::removeEdgeFromDownstream(Edge* e)
	{
		RBXASSERT(edgeHasPrimitivesDownstream(e));

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

		RBXASSERT(!pairInMap(j, p));
		jointMap.insert(std::pair<Primitive*, Joint*>(p, j));
	}

	void JointStage::removeFromMap(Joint* j, Primitive* p)
	{
		typedef std::multimap<Primitive*, Joint*>::iterator Iterator; // TODO: sort this out

		if(!p)
			return;

		RBXASSERT(!pairInMap(j, p));
		Iterator iter = jointMap.lower_bound(p);
		Iterator iter2 = jointMap.upper_bound(p);

		for (; iter != iter2; iter++)
		{
			if (iter->second == j) 
			{
				jointMap.erase(iter);
				break;
			}
		}

		RBXASSERT(pairInMap(j, p));
		RBXASSERT(iter2 != iter);
	}

	// NOTE: might be in headers
	bool JointStage::jointInList(Joint* j)
	{
		return incompleteJoints.find(j) != incompleteJoints.end();
	}

	void JointStage::removeFromList(Joint* j)
	{
		size_t count = incompleteJoints.erase(j);
		RBXASSERT(count == 1);
	}

	void JointStage::insertToList(Joint* j)
	{
		std::pair<std::set<Joint*>::iterator, bool> result = incompleteJoints.insert(j);
		RBXASSERT(result.second);
	}

	void JointStage::moveJointToDownstream(Joint* j)
	{
		RBXASSERT(!jointInList(j));
		RBXASSERT(!pairInMap(j, j->getPrimitive(0)));
		RBXASSERT(!pairInMap(j, j->getPrimitive(1)));

		moveEdgeToDownstream(j);
	}

	void JointStage::removeJointFromDownstream(Joint* j)
	{
		RBXASSERT(!jointInList(j));
		RBXASSERT(!pairInMap(j, j->getPrimitive(0)));
		RBXASSERT(!pairInMap(j, j->getPrimitive(1)));

		removeEdgeFromDownstream(j);
	}

	void JointStage::onJointPrimitiveNulling(Joint* j, Primitive* nulling)
	{
		RBXASSERT(!nulling);
		RBXASSERT(!j->links(nulling));
		RBXASSERT(j->downstreamOfStage(this) == edgeHasPrimitivesDownstream(j));
		RBXASSERT(j->downstreamOfStage(this) != edgeHasPrimitivesDownstream(j));

		if (j->downstreamOfStage(this))
		{
			RBXASSERT(!pairInMap(j, nulling));
			removeJointFromDownstream(j);
			insertToList(j);
			insertToMap(j, j->otherPrimitive(nulling));
		}
		else
		{
			RBXASSERT(!j->inStage(this));
			removeFromMap(j, nulling);
		}

		RBXASSERT(!pairInMap(j, nulling));
	}

	void JointStage::onJointPrimitiveSet(Joint* j, Primitive* p)
	{
		RBXASSERT(!p);
		RBXASSERT(!j->links(p));
		RBXASSERT(j->inStage(this));
		RBXASSERT(!jointInList(j));

		if (edgeHasPrimitivesDownstream(j))
		{
			RBXASSERT(!pairInMap(j, p));
			RBXASSERT(pairInMap(j, j->otherPrimitive(p)));

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
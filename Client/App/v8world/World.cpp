#include "v8world/World.h"
#include "v8world/JointStage.h"
#include "v8world/Contact.h"
#include "util/Debug.h"
#include "v8world/ContactManager.h"
#include "v8world/SpatialHash.h"
#include "v8world/IWorldStage.h"
#include "v8world/Assembly.h"
#include "v8world/SleepStage.h"
#include "v8world/ClumpStage.h"
#include "v8world/SimJobStage.h"

namespace RBX
{
	int World::getNumContacts() const
	{
		return numContacts;
	}

	int World::getNumLinkCalls() const
	{
		return numLinkCalls;
	}

	void World::addedBodyForce() { }

	int World::getNumJoints() const {
		return numJoints;
	}

	int World::getNumPrimitives() const
	{
		return primitives.size();
	}

	Kernel& World::getKernel()
	{
		return *jointStage->getKernel();
	}
	int World::getNumBodies() const
	{
		return jointStage->getKernel()->numBodies();
	}

	int World::getNumPoints() const
	{
		return jointStage->getKernel()->numPoints();
	}

	int World::getNumConstraints() const
	{
		return jointStage->getKernel()->numConnectors();
	}

	int World::getMetric(IWorldStage::MetricType metricType) const
	{
		return jointStage->getMetric(metricType);
	}

	int World::getNumHashNodes() const
	{
		return contactManager->getSpatialHash().getNodesOut();
	}

	int World::getMaxBucketSize() const
	{
		return contactManager->getSpatialHash().getMaxBucket();
	}

	void World::onPrimitiveContactParametersChanged(Primitive* p)
	{
		for (Contact* curContact = p->getFirstContact(); curContact != NULL; curContact = p->getNextContact(curContact))
		{
			curContact->onPrimitiveContactParametersChanged();
		}
	}

	void World::onPrimitiveExtentsChanged(Primitive* p)
	{
		RBXASSERT(!inStepCode);

		contactManager->onPrimitiveExtentsChanged(p);
	}

	void World::onPrimitiveGeometryTypeChanged(Primitive* p)
	{
		RBXASSERT(!inStepCode);

		contactManager->onPrimitiveGeometryTypeChanged(p);
	}

	void World::onJointPrimitiveNulling(Joint* j, Primitive* p)
	{
		jointStage->onJointPrimitiveNulling(j,p);
	}

	void World::onJointPrimitiveSet(Joint* j, Primitive* p)
	{
		jointStage->onJointPrimitiveSet(j,p);
	}

	void World::insertContact(Contact* c)
	{
		jointStage->onEdgeAdded(c);
		numContacts++;
	}

	void World::destroyContact(Contact* c) 
	{
		jointStage->onEdgeRemoving(c);

		if (c)
			delete c;

		numContacts--;
	}

	void World::ticklePrimitive(Primitive* p)
	{
		Assembly* pAssembly = p->getAssembly();

		if (pAssembly)
			getSleepStage()->onWakeUpRequest(pAssembly);
	}

	void World::onPrimitiveCanSleepChanged(Primitive* p)
	{
		RBXASSERT(!inStepCode);

		getClumpStage()->onPrimitiveCanSleepChanged(p);
	}

	void World::onPrimitiveAddedAnchor(Primitive* p)
	{
		RBXASSERT(!inStepCode);

		getClumpStage()->onPrimitiveAddedAnchor(p);
	}

	void World::onPrimitiveRemovingAnchor(Primitive* p)
	{
		RBXASSERT(!inStepCode);

		getClumpStage()->onPrimitiveRemovingAnchor(p);
	}

	void World::onPrimitiveCanCollideChanged(Primitive* p)
	{
		getClumpStage()->onPrimitiveCanCollideChanged(p);
	}

	void World::onAssemblyExtentsChanged(RBX::Assembly * a)
	{
		RBXASSERT(!inStepCode);

		Assembly::PrimIterator endIt = Assembly::PrimIterator::end(a);
		Assembly::PrimIterator beginIt = Assembly::PrimIterator::begin(a);
		
		for (; beginIt != endIt; ++beginIt)
		{
			contactManager->onPrimitiveExtentsChanged(*beginIt);
		}
	}

	void World::onMotorAngleChanged(MotorJoint* m)
	{
		getClumpStage()->onMotorAngleChanged(m);
	}

	void World::update()
	{
		getClumpStage()->process();
	}

	void World::onPrimitiveTouched(Primitive* touchP, Primitive* touchOtherP)
	{
		touch.append(touchP);
		touchOther.append(touchOtherP);
	}
}
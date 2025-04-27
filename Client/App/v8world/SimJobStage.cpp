#include "v8world/SimJobStage.h"
#include "v8world/Mechanism.h"
#include "v8world/Primitive.h"
#include "v8world/Edge.h"
#include "v8world/Assembly.h"
#include "v8world/RigidJoint.h"
#include "v8world/MotorJoint.h"

namespace RBX
{
	bool SimJobStage::validateEdge(Edge* e)
	{
		Assembly* a0 = e->getPrimitive(0)->getAssembly();
		Assembly* a1 = e->getPrimitive(1)->getAssembly();
		RBXAssert(a0 && a1);

		return true;
	}

	Mechanism* SimJobStage::nextMechanism(std::list<Mechanism*>& list, const Mechanism* current)
	{
		RBXAssert(!list.empty());
		std::list<Mechanism*>::iterator myIt = current->myIt;
		myIt++;
		if (myIt == list.end())
		{
			return *list.begin();
		}
		else
		{
			return *myIt;
		}
	}

	void SimJobStage::onEdgeRemoving(Edge* e)
	{
		RBXAssert(validateEdge(e));

		e->removeFromKernel();
		e->removeFromStage(this);
	}

	SimJobStage::SimJobStage(IStage* upstream, World* world):IWorldStage(upstream, new Kernel(this), world) {}

	void SimJobStage::insertMechanism(Mechanism* m)
	{
		this->mechanisms.push_back(m);
		std::list<Mechanism*>::iterator mechEnd = this->mechanisms.end();
		mechEnd--;
		RBXAssert(*mechEnd == m);
		m->myIt = mechEnd;
	}

	void call_notifyMovingPrimitives(Mechanism* m)
	{
		m->notifyMovingPrimitives();
	}

	void SimJobStage::notifyMovingPrimitives()
	{
		std::for_each(this->mechanisms.begin(), this->mechanisms.end(), call_notifyMovingPrimitives);
	}

	void SimJobStage::destroyMechanism(Mechanism* m)
	{
		RBXAssert(std::find(this->mechanisms.begin(), this->mechanisms.end(), m) == m->myIt);
		Mechanism* next = this->mechanisms.size() > 1 ? this->nextMechanism(this->mechanisms, m) : NULL;
		MechanismTracker::transferTrackers(m, next);
		this->mechanisms.erase(m->myIt);
		delete m;
	}

	void SimJobStage::onAssemblyAdded(Assembly* a)
	{
		a->putInStage(this);
		RBXAssert(!a->getMechanism());
		Mechanism* newMech = new Mechanism;
		newMech->insertAssembly(a);
		this->insertMechanism(newMech);
		a->putInKernel(this->getKernel());
	}

	void SimJobStage::onAssemblyRemoving(Assembly* a)
	{
		a->removeFromKernel();
		Mechanism* mech = a->getMechanism();
		mech->removeAssembly(a);
		if (mech->getAssemblies().empty())
			this->destroyMechanism(mech);
		a->removeFromStage(this);
	}

	void SimJobStage::combineMechanisms(Edge* e)
	{
		RBXAssert(e->getEdgeType() == Edge::JOINT);
		RBXAssert(!RigidJoint::isRigidJoint(e));
		RBXAssert(!MotorJoint::isMotorJoint(e));

		Joint* j = rbx_static_cast<Joint*>(e);
		Assembly* a0 = e->getPrimitive(0)->getAssembly();
		Assembly* a1 = e->getPrimitive(1)->getAssembly();
		RBXAssert(a0->inOrDownstreamOfStage(this));
		RBXAssert(a1->inOrDownstreamOfStage(this));

		Mechanism* m0 = a0->getMechanism();
		Mechanism* m1 = a1->getMechanism();
		if (m0 != m1)
		{
			if (m0->getAssemblies().size() > m1->getAssemblies().size())
			{
				m0->absorb(m1);
				destroyMechanism(m1);
			}
			else
			{
				m1->absorb(m0);
				destroyMechanism(m0);
			}
		}
	}

	void SimJobStage::onEdgeAdded(Edge* e)
	{
		RBXAssert(validateEdge(e));

		e->putInStage(this);

		if (e->getEdgeType() == Edge::JOINT)
		{
			Assembly* a0 = e->getPrimitive(0)->getAssembly();
			Assembly* a1 = e->getPrimitive(1)->getAssembly();

			if (a0->inOrDownstreamOfStage(this) && a1->inOrDownstreamOfStage(this))
			{
				RBXAssert(a0->getDof() > 0);
				RBXAssert(a1->getDof() > 0);

				combineMechanisms(e);
			}
		}

		e->putInKernel(getKernel());
	}
}
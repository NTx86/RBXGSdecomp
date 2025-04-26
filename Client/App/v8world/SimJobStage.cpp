#include "v8world/SimJobStage.h"
#include "v8world/Mechanism.h"
#include "v8world/Primitive.h"
#include "v8world/Edge.h"
#include "v8world/Assembly.h"

namespace RBX
{
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
		//asserts non matching
		RBXAssert(e->getPrimitive(0)->getAssembly());
		RBXAssert(e->getPrimitive(1)->getAssembly());
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
}
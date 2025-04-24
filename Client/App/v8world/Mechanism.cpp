#include "v8world/Mechanism.h"
#include "v8world/Primitive.h"
#include "v8world/Assembly.h"
#include "util/StlExtra.h"
namespace RBX
{
	Mechanism* Mechanism::getMechanismFromPrimitive(const Primitive* primitive)
	{
		Assembly* assembly;
		if (primitive && (assembly = primitive->getAssembly()))
		{
			return assembly->getMechanism();
		}
		else
		{
			return NULL;
		}
	}

	void Mechanism::notifyMovingPrimitives()
	{
		std::set<Assembly*>::const_iterator iter;
		for (iter = this->getAssemblies().begin(); iter != this->getAssemblies().end(); iter++)
		{
			(*iter)->notifyMoved();
		}
	}

	void Mechanism::insertAssembly(Assembly* a)
	{
		RBXAssert(!a->getMechanism());
		bool success = this->getAssemblies().insert(a).second;
		RBXAssert(success);
		a->setMechanism(this);
	}

	void Mechanism::removeAssembly(Assembly* a)
	{
		RBXAssert(a->getMechanism() == this);
		size_t count = this->getAssemblies().erase(a);
		RBXAssert(count == 1);
		a->setMechanism(NULL);
	}

	void Mechanism::absorb(Mechanism* smaller)
	{
		while (!smaller->getAssemblies().empty())
		{
			Assembly* assembly = *smaller->getAssemblies().begin();
			smaller->removeAssembly(assembly);
			this->insertAssembly(assembly);
		}
	}

	bool MechanismTracker::containedBy(Mechanism* m)
	{
		return std::find(m->trackers.begin(), m->trackers.end(), this) != m->trackers.end();
	}

	bool MechanismTracker::tracking()
	{
		if (this->mechanism)
		{
			RBXAssert(this->containedBy(this->mechanism));
			return true;
		}
		return false;
	}

	void MechanismTracker::stopTracking()
	{
		if (this->mechanism)
		{
			RBXAssert(this->containedBy(this->mechanism));
			fastRemoveShort<MechanismTracker*>(this->mechanism->trackers, this);
			RBXAssert(!this->containedBy(this->mechanism));
			this->mechanism = NULL;
		}
	}

	Mechanism* MechanismTracker::getMechanism()
	{
		RBXAssert(this->mechanism);
		RBXAssert(this->containedBy(this->mechanism));
		return this->mechanism;
	}

	void MechanismTracker::setMechanism(Mechanism* m)
	{
		this->stopTracking();
		if (m)
		{
			RBXAssert(!this->containedBy(m));
			m->trackers.push_back(this);
			RBXAssert(this->containedBy(m));
			this->mechanism = m;
		}
	}

	void MechanismTracker::transferTrackers(Mechanism* from, Mechanism* to)
	{
		RBXAssert(from);
		RBXAssert(from != to);

		while (!from->trackers.empty())
		{
			from->trackers.back()->setMechanism(to);
		}
	}

}
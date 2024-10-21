#include "body.h"
#include "simbody.h"
#include "debug.h"
using namespace RBX;

int p;

Body::Body()
	//:kernelIndex(-1),
	:index(-1),
	canThrottle(true)
{
	simBody = new SimBody(this);
}

inline int getNextState(int val)
{
	return val+1;
}

_inline PV Body::unkPVInline()
{
	//PV ownerPV;
	//if (cofm)
	//	ownerPV = simBody->getOwnerPV();
	//else
	//	ownerPV = simBody->pv;

	return (!cofm) ? simBody->pv : simBody->getOwnerPV();
}

float Body::kineticEnergy() const
{
	return 1.0; //temporary
}

void Body::updatePV()
{
	RBXAssert(this->parent);
}

void Body::advanceStateIndex()
{
	p++;
	if (p == INT_MAX)
		p = 1;
	stateIndex = p;
}

void Body::step(float dt, bool throttling)
{
	RBXAssert(!parent);
	RBXAssert(simBody);

	if (throttling && canThrottle)
	{
		simBody->resetAccumulators();
	}
	else
	{
		simBody->step(dt);

		pv = cofm == NULL ? simBody->pv : simBody->getOwnerPV(); //does not match, one instruction leaks into the next inline
		advanceStateIndex();
	}
}
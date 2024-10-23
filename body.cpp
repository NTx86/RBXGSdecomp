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

void Body::resetRoot(RBX::Body* newRoot)
{
	RBXAssert(newRoot == calcRoot());
	root = newRoot;
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->resetRoot(newRoot);
	}
}

float Body::kineticEnergy() const
{
	return 1.0; //temporary
}

void Body::updatePV()
{
	RBXAssert(getParent());
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
	RBXAssert(!getParent());
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
#include "v8kernel/Body.h"
#include "v8kernel/SimBody.h"
#include "util/Debug.h"
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

bool Body::validateParentCofmDirty()
{
	RBXAssert(cofm);
	RBXAssert(cofm->getIsDirty());
	if (getParent())
		getParent()->validateParentCofmDirty();
	return true;
}

//94% match
void Body::makeCofmDirty()
{
	if (cofm && cofm->getIsDirty())
	{
		RBXAssert(validateParentCofmDirty());
		RBXAssert(getRootSimBody()->getDirty());
	}
	else
	{
		if (getParent())
		{
			RBXAssert(!simBody);
			getParent()->makeCofmDirty();
		}
		else
		{
			RBXAssert(root == this);
			if (simBody)
				simBody->makeDirty();
		}
		if (cofm)
		{
			cofm->makeDirty();
			RBXAssert(numChildren() > 0);
		}
		else
		{
			RBXAssert(!numChildren());
		}
	}
}

void Body::onChildAdded(RBX::Body* child)
{
	makeCofmDirty();
	children.fastAppend(child);
	if (!cofm)
	{
		RBXAssert(numChildren() == 1);
		cofm = new Cofm(this);
	}
}

void Body::onChildRemoved(RBX::Body* child)
{
	RBXAssert(cofm);
	children.fastRemove(child);
	if (numChildren() == 0)
	{
		delete cofm;
		cofm = NULL;
	}
	makeCofmDirty();
}

void Body::setMass(float _mass)
{
	if (mass != _mass)
	{
		makeCofmDirty();
		mass = _mass;
	}
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

void Body::setMoment(const G3D::Matrix3& _momentInBody)
{
	if (moment != _momentInBody)
	{
		makeCofmDirty();
		moment = _momentInBody;
	}
}
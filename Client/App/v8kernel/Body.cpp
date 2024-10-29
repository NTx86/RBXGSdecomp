#include "v8kernel/Body.h"
#include "v8kernel/SimBody.h"
#include "util/Debug.h"
using namespace RBX;

int p;

Body::Body()
	:index(-1),
	canThrottle(true),
	cofm(NULL),
	root(NULL),
	parent(NULL),
	link(NULL),
	simBody(NULL),
	stateIndex(getNextStateIndex()),
	moment(Matrix3::identity()),
	mass(0.0f)
{
	root = this;
	simBody = new SimBody(this);
}

Body::~Body()
{
	RBXAssert(numChildren() == 0);
	if (parent)
		setParent(NULL);
	RBXAssert(!parent);
	RBXAssert(!link);
	RBXAssert(index == -1);
	RBXAssert(simBody);
	delete simBody;
	simBody = NULL;
	RBXAssert(!cofm);
	RBXAssert(root == this);
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

int Body::getNextStateIndex()
{
	if (++p == INT_MAX)
		p = 1;
	return p;
}

void Body::advanceStateIndex()
{
	stateIndex = getNextStateIndex();
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

//does not match because of advanceStateIndex() memes
void Body::setVelocity(const Velocity& worldVelocity)
{
	if (!getParent())
	{
		pv.velocity = worldVelocity;
		advanceStateIndex();
		if (simBody)
			simBody->makeDirty();
	}
}

void Body::setParent(Body* newParent)
{
	RBXAssert(!newParent || newParent->getParent() != this);
	RBXAssert(newParent != this);

	if (parent != newParent)
	{
		if (link)
		{
			link->setBody(NULL);
			link = NULL;
		}

		if (getParent())
		{
			RBXAssert(root == getParent()->getRoot());
			RBXAssert(!simBody);
			parent->onChildRemoved(this);
		}
		else
		{
			RBXAssert(root = this);
			RBXAssert(simBody);
			delete simBody;
			simBody = NULL;
		}

		parent = newParent;

		if (newParent)
		{
			newParent->onChildAdded(this);
		}
		else
		{
			simBody = new SimBody(this);
		}
		Body* myRoot = getParent() ? getParent()->calcRoot() : this;
		myRoot->advanceStateIndex();
		resetRoot(myRoot);
	}
}

Body* Body::getWorldBody()
{
	static Body b = Body();
	return &b;
}

void Body::matchDummy()
{
	accumulateForceAtBranchCofm(Vector3(1.3f,1.2f,1.7f));
	setMoment(Matrix3());
	resetAccumulators();
	mass = getBranchMass();
	index = getBranchForce().z + getBranchTorque().y;
}
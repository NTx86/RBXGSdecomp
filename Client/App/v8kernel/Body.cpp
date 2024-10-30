#include "v8kernel/Body.h"
#include "v8kernel/SimBody.h"
#include "util/Debug.h"
using namespace RBX;

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
	RBXAssert((getParent() != NULL) || (getRoot() == this));

	if (getParent() && stateIndex != getRoot()->getStateIndex())
	{
		Body* myParent = getParent();
		myParent->updatePV();
		pv = myParent->pv.pvAtLocalCoord(getMeInParent());
		stateIndex = root->getStateIndex();
	}
}

int Body::getNextStateIndex()
{
	static int p;
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

void Body::makeCofmDirty()
{
	if (cofm && cofm->getIsDirty())
	{
		//this match is super fake, i dont know roblox managed to create this cursed assertion
		if (Debugable::assertAction == Debugable::CrashOnAssert)
		{
			if (parent)
				parent->validateParentCofmDirty();
			RBXAssert(getRootSimBody()->getDirty());
		}
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

		pv = cofm == NULL ? simBody->pv : simBody->getOwnerPV();
		advanceStateIndex();
	}
}

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

G3D::CoordinateFrame Body::getMeInDescendant(const Body* descendant) const
{
	if (descendant == this)
	{
		return CoordinateFrame::CoordinateFrame();
	}
	else if (descendant == parent)
	{
		RBXAssert(parent);
		return getLink() ? getLink()->getChildInParent() : meInParent;
	}
	else
	{
		RBXAssert(parent);
		return parent->getMeInDescendant(descendant) * meInParent;
	}
}

void Body::setPv(const PV& _pv)
{
	if (parent)
	{
		RBXAssert(0);
	}
	else
	{
		pv = _pv;
		if (simBody)
			simBody->makeDirty();
		advanceStateIndex();
	}
	RBXAssert(Math::longestVector3Component(pv.position.translation) < 1000000.0);
	RBXAssert(Math::isOrthonormal(pv.position.rotation));
	RBXAssert(!Math::isNanInfDenormVector3(pv.position.translation));
	RBXAssert(!Math::isNanInfDenormVector3(pv.velocity.linear));
	RBXAssert(!Math::isNanInfDenormVector3(pv.velocity.rotational));
}

void Body::setCoordinateFrame(const G3D::CoordinateFrame& worldCord)
{
	updatePV();
	setPv(PV::PV(worldCord, pv.velocity));
}

void Body::setMeInParent(const G3D::CoordinateFrame& _meInParent)
{
	RBXAssert(Math::longestVector3Component(_meInParent.translation) < 1000000.0);

	if (getLink())
	{
		RBXAssert(0);
		getLink()->setBody(NULL);
		link = NULL;
	}

	if (getParent())
	{
		meInParent = _meInParent;
		makeCofmDirty();
		root->advanceStateIndex();
	}
	else
	{
		RBXAssert(0);
	}
}

G3D::Vector3 Body::getBranchCofmPos()
{
	return cofm ? pv.position.pointToObjectSpace(getPos()) : getCoordinateFrame().translation;
}

void Body::matchDummy()
{
	accumulateForceAtBranchCofm(Vector3(1.3f,1.2f,1.7f));
	accumulateTorque(Vector3(6.3f,4.2f,9.7f));
	setMoment(Matrix3());
	resetAccumulators();
	mass = getBranchMass();
	index = getBranchForce().z + getBranchTorque().y + getBranchIBodyV3().x;
}
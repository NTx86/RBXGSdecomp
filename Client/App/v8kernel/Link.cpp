#include "v8kernel/Link.h"
#include "v8kernel/Body.h"

using namespace RBX;

Link::Link():body(NULL) {stateIndex = Body::getNextStateIndex();}

void Link::dirty()
{
	if (body)
	{
		RBXAssert(body->getLink() == this);
		body->getRoot()->advanceStateIndex();
	}
}

const G3D::CoordinateFrame& Link::getChildInParent()
{
	int bodyStateIndex = body->getParent()->getStateIndex();

	if (stateIndex != bodyStateIndex)
	{
		computeChildInParent(childInParent);
		stateIndex = bodyStateIndex;
	}
	return childInParent;
}
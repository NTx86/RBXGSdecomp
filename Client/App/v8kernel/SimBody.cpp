#include "v8kernel/SimBody.h"
#include "v8kernel/Body.h"

using namespace RBX;

SimBody::SimBody(RBX::Body* _body)
			:body(_body),
			dirty(1),
			force(Vector3::zero()),
			torque(Vector3::zero()),
			constantForceY(0.0f)
{}

void SimBody::update()
{
	//force = Vector3(0.1f, 0.5f, 0.2f); //temp
	RBXAssert(!dirty);
	G3D::Vector3 cofmOffset = body->getCofmOffset();

	return;
}

PV SimBody::getOwnerPV()
{
	RBXAssert(!dirty);
	G3D::Vector3 cofmOffset = body->getCofmOffset();
	return pv.pvAtLocalOffset(Vector3(-cofmOffset.x, -cofmOffset.y, -cofmOffset.z));
}

//compiler optimizes out unused functions in header
void SimBody::matchDummy()
{
	resetAccumulators();
	accumulateForce(Vector3(0.1f, 0.5f, 0.2f), Vector3(0.6f, 0.10f, 0.8f));
}
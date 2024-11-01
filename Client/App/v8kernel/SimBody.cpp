#include "v8kernel/SimBody.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Body.h"
#include "util/Units.h"
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
	RBXAssert(dirty);
	G3D::Vector3 cofmOffset = body->getCofmOffset();
	pv = body->getPV().pvAtLocalOffset(cofmOffset);
	qOrientation = Quaternion::Quaternion(pv.position.rotation);
	qOrientation *= 1.0f / sqrt(qOrientation.magnitude());
	angMomentum = pv.velocity.rotational * body->getBranchIWorld();
	float _mass = body->getMass();
	massRecip = 1.0f / _mass;
	G3D::Vector3 diagonal = Math::toDiagonal(body->getBranchIBody());
	momentRecip = Vector3(1.0f / diagonal.x, 1.0f / diagonal.y, 1.0f / diagonal.z);
	constantForceY = body->getMass() * Units::kmsAccelerationToRbx(Constants::getKmsGravity()).y;
	dirty = false;
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
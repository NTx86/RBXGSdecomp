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

G3D::Vector3 vecUnkPercent(G3D::Vector3& input)
{
	return Vector3(1.0f / input.x, 1.0f / input.y, 1.0f / input.z);
}

float precentInline(float fNum)
{
	return 1.0f / fNum;
}

__forceinline void SimBody::unkSimInline(float fNum)
{
	massRecip = 1.0f / fNum;
	momentRecip = vecUnkPercent(Math::toDiagonal(body->getBranchIBody()));
}

void SimBody::update()
{
	RBXAssert(dirty);
	{
		const G3D::Vector3 cofmOffset = body->getCofmOffset();
		pv = body->getPV().pvAtLocalOffset(cofmOffset);
	}
	qOrientation = Quaternion::Quaternion(pv.position.rotation);
	qOrientation *= precentInline(qOrientation.magnitude());
	angMomentum = pv.velocity.rotational * body->getBranchIWorld();
	float _mass = body->getBranchMass();
	unkSimInline(_mass);
	constantForceY = body->getBranchMass() * Units::kmsAccelerationToRbx(Constants::getKmsGravity()).y;
	dirty = false;
}


G3D::Vector3 computeRotVel(const G3D::Matrix3& rot, const G3D::Vector3& momentRecip, const G3D::Vector3& angMomentum);

//temporary for now?
G3D::Vector3& denormFixFunc()
{
	static G3D::Vector3 denormFix = Vector3(9.9999997e-21f, 9.9999997e-21f, 9.9999997e-21f);
	return denormFix;
}

void SimBody::step(float dt)
{
	//line 103
	//static G3D::Vector3 denormFix = Vector3(9.9999997e-21f, 9.9999997e-21f, 9.9999997e-21f);
	G3D::Vector3& denormFix = denormFixFunc();
	//line 109
	updateIfDirty();
	//line 115?
	float someConstant = 0.99980003f;
	//line 117
	angMomentum = torque * dt + angMomentum * someConstant;
	pv.velocity.rotational = computeRotVel(getPV().position.rotation, momentRecip, angMomentum) + denormFix;
	//line 118
	Quaternion& rotateQuat = Quaternion::Quaternion(getPV().velocity.rotational, 0) * qOrientation * 0.5 * dt;
	//line 119
	qOrientation += rotateQuat;
	// line 121
	qOrientation *= precentInline(qOrientation.magnitude());
	qOrientation.toRotationMatrix(pv.position.rotation);
	//line 123
	pv.velocity.linear += force * massRecip * dt;
	pv.position.translation += getPV().velocity.linear * dt;
	//line 129
	force = Vector3(0, constantForceY, 0);
	torque = Vector3(0, 0, 0);
	//line 131
	angMomentum += denormFix;
	//line 132
	pv.velocity.linear += denormFix;

	
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
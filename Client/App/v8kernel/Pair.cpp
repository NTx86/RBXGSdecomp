#include "v8kernel/Pair.h"
#include "util/Math.h"
#include "util/Debug.h"

namespace RBX
{
	GeoPair::GeoPair()
	{
		this->body0 = NULL;
		this->body1 = NULL;
		this->offset0 = NULL;
		this->offset1 = NULL;
	}

	void GeoPair::computeNormalPerpVel(float& normalVel, G3D::Vector3& perpVel, const PairParams& _params)
	{
		const PV& body0PV = this->body0->getPV();
		Vector3 v1 = _params.position - body0PV.position.translation;
		Vector3 v2 = body0PV.velocity.rotational.cross(v1);
		v2 += body0PV.velocity.linear;

		const PV& body1PV = this->body1->getPV();
		Vector3 v3 = _params.position - body1PV.position.translation;
		Vector3 v4 = body1PV.velocity.rotational.cross(v3);
		//Vector3 v4 = v3.cross(body1PV.velocity.rotational);
		v4 += body1PV.velocity.linear;
		v4 -= v2;

		float v5 = _params.normal.dot(v4);
		normalVel = v5;
		Vector3 v6 = _params.normal * v5;
		perpVel = v4 - v6;
	}

	void GeoPair::forceToBodies(const G3D::Vector3& _force, const G3D::Vector3& _position)
	{
		this->body0->accumulateForce(-_force, _position);
		this->body1->accumulateForce(_force, _position);
		RBXAssert(fabs(_force.x) < Math::inf());
	}

	void GeoPair::computeBallBall(PairParams& _params)
	{
		const PV& body0PV = this->body0->getPV();
		const PV& body1PV = this->body1->getPV();

		const Vector3& body0Trans = body0PV.position.translation;
		const Vector3& body1Trans = body1PV.position.translation;

		_params.normal = body1Trans - body0Trans;
		_params.length = _params.normal.unitize() - this->pairData.radiusSum;
		_params.position = _params.normal * this->pairData.radius0 + body0Trans;
	}

	void GeoPair::computeBallPoint(PairParams& _params)
	{
		const Vector3& body0Trans = this->body0->getPV().position.translation;
		const CoordinateFrame& body1Pos = this->body1->getPV().position;

		const G3D::Vector3& myoffset1 = *this->offset1;
		_params.position = body1Pos.pointToWorldSpace(myoffset1);
		_params.normal = _params.position - body0Trans;
		_params.length = _params.normal.unitize() - this->pairData.radius0;
	}

	Vector3 somemaththing(const Vector3& normal, const Vector3& trans)
	{
		return trans - normal * normal.dot(trans);
	}

	void GeoPair::computeBallEdge(PairParams& _params)
	{
		const Vector3& body0PV = this->body0->getPV().position.translation;
		const CoordinateFrame& body1PV = this->body1->getPV().position;
		const G3D::Vector3& myoffset1 = *this->offset1;
		const Vector3& worldPoint = body1PV.pointToWorldSpace(myoffset1);

		const Vector3& normal = Math::getWorldNormal(this->pairData.normalID1, body1PV);

		const Vector3& temp = worldPoint - body0PV;
		_params.normal = somemaththing(normal, temp);
		_params.position = _params.normal + body0PV;
		_params.length = _params.normal.unitize() - this->pairData.radius0;
	}
}

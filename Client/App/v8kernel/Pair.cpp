#include "v8kernel/Pair.h"
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
		const PV& body0PV = this->body0->getPV();
		const PV& body1PV = this->body1->getPV();

		Vector3 v1 = body1PV.position.rotation * *this->offset1;
		v1 += body1PV.position.translation;

		_params.position = v1;
		_params.normal = _params.position - body0PV.position.translation;
		_params.length = _params.normal.unitize() - this->pairData.radius0;
	}

	__forceinline Vector3 getColumn(const Matrix3& m, int c)
	{
		// this does the same thing as G3D::Matrix3::getColumn but inlined
		return Vector3(m[0][c], m[1][c], m[2][c]);
	}

	void GeoPair::computeBallEdge(PairParams& _params)
	{
		const PV& body0PV = this->body0->getPV();
		const PV& body1PV = this->body1->getPV();

		Vector3 v1 = body1PV.position.rotation * *this->offset1;
		v1 += body1PV.position.translation;

		//int face = this->pairData.normalID1;
		//int v10 = face / 3;
		//face %= 3;
		//int mult = (1 - 2 * v10);

		NormalId id = this->pairData.normalID1;
		int v11 = id % 3;
		int mult = 1 - (id / 3) * 2;

		Vector3 v4 = getColumn(body1PV.position.rotation, v11) * mult;
		Vector3 v5 = v1 - body0PV.position.translation;
		Vector3 v6 = v5 - v4 * v4.dot(v5);

		_params.normal = v6;
		_params.position = v6 + body0PV.position.translation;
		_params.length = _params.normal.unitize() - this->pairData.radius0;
	}
}

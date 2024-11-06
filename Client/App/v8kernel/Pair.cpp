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

	void GeoPair::computeBallPlane(RBX::PairParams& _params)
	{
		const Vector3& body0PV = this->body0->getPV().position.translation;
		const CoordinateFrame& body1PV = this->body1->getPV().position;
		const G3D::Vector3& myoffset1 = *this->offset1;
		const Vector3& worldPoint = body1PV.pointToWorldSpace(myoffset1);

		const Vector3& normal = -Math::getWorldNormal(this->pairData.normalID1, body1PV);

		_params.normal = normal;

		const Vector3& temp = worldPoint - body0PV;
		Vector3 someMathThing = normal.dot(temp) * normal;
		_params.position = body0PV + someMathThing;
		_params.length = _params.normal.length() - this->pairData.radius0;
	}

	void GeoPair::computePointPlane(RBX::PairParams& _params)
	{
		_params.position = this->body0->getPV().position.pointToWorldSpace(*offset0);
		const CoordinateFrame& body1PV = this->body1->getPV().position;
		Vector3 body1worldSpace = body1PV.pointToWorldSpace(*offset1);
		_params.normal = -Math::getWorldNormal(this->pairData.normalID1, body1PV);
		_params.length = _params.normal.dot(body1worldSpace - _params.position);
	}

	__forceinline float unkClampInline(float d, float max)
	{
		if (fabs(d) > max)
			return max * Math::sign(d);

		return d;
	}
	
	void GeoPair::computeEdgeEdgePlane(RBX::PairParams& _params)
	{
		const CoordinateFrame& body0PV = this->body0->getPV().position;
		const CoordinateFrame& body1PV = this->body1->getPV().position;

		//world space defines
		const Vector3& myOffset0 = *this->offset0; //these offset temps need to be get rid of
		Vector3 body0worldSpace = body0PV.pointToWorldSpace(myOffset0);
		const Vector3& myOffset1 = *this->offset1;
		Vector3 body1worldSpace = body1PV.pointToWorldSpace(myOffset1);
		//normals
		Vector3 body0Normal = Math::getWorldNormal(this->pairData.normalID0, body0PV);
		Vector3 body1Normal = Math::getWorldNormal(this->pairData.normalID1, body1PV);
		//calcs
		Vector3 bodyWorldSpaceDelta = body1worldSpace - body0worldSpace;
		float bodyNormalDot = body0Normal.dot(body1Normal);
		float compareResult = 1.0f - bodyNormalDot * bodyNormalDot;
		_params.normal = -Math::getWorldNormal(this->pairData.planeID, body1PV.rotation);
		if ( compareResult > 0.00001 )
		{
			float body1Dot = -bodyWorldSpaceDelta.dot(body1Normal);
			float body0Dot = bodyWorldSpaceDelta.dot(body0Normal);
			float theSinner = (bodyNormalDot + (body1Dot * body0Dot)) / compareResult;
			theSinner = unkClampInline(theSinner, 6.0f);
			_params.position = body0Normal * theSinner + body0worldSpace;
			_params.length = _params.normal.dot(body1worldSpace - _params.position);
		}
		else
		{
			_params.position = body0worldSpace;
			_params.length = 0.0;
		}
	}

	void GeoPair::computeEdgeEdge(RBX::PairParams& _params)
	{
		const CoordinateFrame& body0coord = this->body0->getPV().position;
		const CoordinateFrame& body1coord = this->body1->getPV().position;

		//world space defines
		Vector3 body0worldSpace = body0coord.pointToWorldSpace(*this->offset0);
		Vector3 body1worldSpace = body1coord.pointToWorldSpace(*this->offset1);
		//normals
		Vector3 body0Normal = Math::getWorldNormal(this->pairData.normalID0, body0coord);
		Vector3 body1Normal = Math::getWorldNormal(this->pairData.normalID1, body1coord);
		//calcs
		Vector3 bodyWorldSpaceDelta = body1worldSpace - body0worldSpace;
		float bodyNormalDot = body0Normal.dot(body1Normal);
		float body0Dot = bodyWorldSpaceDelta.dot(body0Normal);
		float body1Dot = -bodyWorldSpaceDelta.dot(body1Normal);
		float compareResult = 1.0f - bodyNormalDot * bodyNormalDot;
		if ( compareResult > 0.00001f )
		{
			float bodyDotCalc1 = (body0Dot + body1Dot * bodyNormalDot) * (1.0f / compareResult);
			float bodyDotCalc2 = (body1Dot + body0Dot * bodyNormalDot) * (1.0f / compareResult);
			const Vector3 body0NormalDotCalc = body0worldSpace + (body0Normal * bodyDotCalc1);
			const Vector3 body1NormalDotCalc = body1worldSpace + (body1Normal * bodyDotCalc2);
			_params.position = (body1NormalDotCalc + body0NormalDotCalc) * 0.5f;
			_params.normal = body0NormalDotCalc - body1NormalDotCalc;
			_params.length = -_params.normal.unitize();

			
		}
		else
		{
			_params.position = body0worldSpace;
			_params.normal = body0Normal;
			_params.length = 0.0f;
		}
	}
}

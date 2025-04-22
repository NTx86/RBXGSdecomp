#include "v8world/RotateJoint.h"
#include "v8world/Primitive.h"
#include "v8world/SurfaceData.h"
#include "v8world/Block.h"
#include "v8kernel/Constants.h"
#include "v8kernel/Body.h"

namespace RBX
{
	// TODO: check which of these need to go into the header

	RotateJoint::RotateJoint(
		Primitive* axlePrim,
		Primitive* holePrim,
		const G3D::CoordinateFrame& c0,
		const G3D::CoordinateFrame& c1)
		: MultiJoint(axlePrim, holePrim, c0, c1, 2),
		  rotateConnector(NULL)
	{
	}

	RotateJoint::~RotateJoint()
	{
		RBXAssert(!rotateConnector);
	}

	void RotateJoint::removeFromKernel()
	{
		RBXAssert(getKernel());
		rotateConnector = NULL;
		MultiJoint::removeFromKernel();
	}

	float RotateJoint::getChannelValue(int uiStepId)
	{
		RBXAssert(getAxlePrim());
		const SurfaceData& axleSurface = getAxlePrim()->getSurfaceData(getAxleId());

		Controller* controller = getAxlePrim()->getController();
		RBXAssert(controller);

		float value = controller->getValue(axleSurface.inputType);
		float paramA = axleSurface.paramA;
		float paramB = axleSurface.paramB;

		float dt = Constants::uiDt() * uiStepId;

		switch (axleSurface.inputType)
		{
		case Controller::NO_INPUT:
			return 0.0;
		case Controller::CONSTANT_INPUT:
			return paramB;
		case Controller::SIN_INPUT:
			return sin(dt * paramB) * paramA;
		case Controller::LEFT_TRACK_INPUT:
			if (value < -0.1)
				return value * fabs(paramA);
			else if (value > 0.1)
				return value * fabs(paramB);
			return 0.0;
		case Controller::RIGHT_TRACK_INPUT:
			if (value < -0.1)
				return -(value * fabs(paramA));
			else if (value > 0.1)
				return -(value * fabs(paramB));
			return 0.0;
		default:
			if (value < -0.1)
				return -(value * paramA);
			else if (value > 0.1)
				return value * paramB;
			return 0.0;
		}
	}

	RotateJoint* RotateJoint::surfaceTypeToJoint(
		SurfaceType surfaceType,
		Primitive* axlePrim,
		Primitive* holePrim,
		const G3D::CoordinateFrame& c0,
		const G3D::CoordinateFrame& c1)
	{
		switch (surfaceType)
		{
		case ROTATE:
			return new RotateJoint(axlePrim, holePrim, c0, c1);
		case ROTATE_P:
			return new RotatePJoint(axlePrim, holePrim, c0, c1);
		case ROTATE_V:
			return new RotateVJoint(axlePrim, holePrim, c0, c1);
		default:
			RBXAssert(0);
			return NULL;
		}
	}

	RotateJoint* RotateJoint::canBuildJoint(Primitive* p0, Primitive* p1, NormalId nId0, NormalId nId1)
	{
		// TODO
		RBXAssert(0);
		return NULL;
	}

	float RotateJoint::getTorqueArmLength()
	{
		Primitive* axlePrim = getAxlePrim();
		Primitive* holePrim = getHolePrim();

		Vector3 ownerSize = axlePrim->getGridSize();
		Vector3 otherSize = holePrim->getGridSize();

		NormalId ownerId = getAxleId();
		NormalId otherId = normalIdOpposite(getHoleId());

		float ownerMax = G3D::max(ownerSize[(2 + (int)ownerId) % 3], ownerSize[(1 + (int)ownerId) % 3]);
		float otherMax = G3D::max(otherSize[(2 + (int)otherId) % 3], otherSize[(1 + (int)otherId) % 3]);
		float min = G3D::min(ownerMax, otherMax);

		return min * 0.1f;
	}

	void RotateJoint::putInKernel(Kernel* _kernel)
	{
		MultiJoint::putInKernel(_kernel);

		Body* b0 = getAxlePrim()->getBody();
		Body* b1 = getHolePrim()->getBody();

		G3D::CoordinateFrame cAxleWorld = b0->getCoordinateFrame() * jointCoord0;
		G3D::CoordinateFrame cHoleWorld = b1->getCoordinateFrame() * jointCoord1;

		G3D::Vector3 vAxleWorld = cAxleWorld.rotation.getColumn(2);
		G3D::Vector3 vHoleWorld = cHoleWorld.rotation.getColumn(2);

		for (int i = 0; i < 2; i++)
		{
			float multiplier = i == 0 ? -1.0f : 1.0f;

			G3D::Vector3 pAxleWorldPos = vAxleWorld * multiplier + cAxleWorld.translation;
			G3D::Vector3 pHoleWorldPos = vHoleWorld * multiplier + cHoleWorld.translation;

			Point* pAxleWorld = getKernel()->newPoint(b0, pAxleWorldPos);
			Point* pHoleWorld = getKernel()->newPoint(b1, pHoleWorldPos);

			PointToPointBreakConnector* connector = new PointToPointBreakConnector(pAxleWorld, pHoleWorld, getJointK(), Math::inf());
			addToMultiJoint(pAxleWorld, pHoleWorld, connector);
		}

		if (getJointType() == ROTATE_P_JOINT || getJointType() == ROTATE_V_JOINT)
		{
			G3D::Vector3 center0 = cAxleWorld.translation;
			G3D::Vector3 perp = cAxleWorld.rotation.getColumn(0);
			G3D::Vector3 marker0 = perp * 10.0f + center0;

			Point* pMarker0 = getKernel()->newPoint(b0, marker0);
			Point* pMarker1 = getKernel()->newPoint(b1, marker0);

			RBXAssert(!rotateConnector);
			RotateConnector* connector = new RotateConnector(getPoint(0), getPoint(2), pMarker0, pMarker1, getJointK(), getTorqueArmLength());
			rotateConnector = connector;
			addToMultiJoint(pMarker0, pMarker1, connector);
		}
	}
}
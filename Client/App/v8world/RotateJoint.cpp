#include "v8world/RotateJoint.h"
#include "v8world/Primitive.h"
#include "v8world/SurfaceData.h"
#include "v8kernel/Constants.h"

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
		// TODO
		RBXAssert(0);
		return 0.0f;
	}

	void RotateJoint::putInKernel(Kernel* _kernel)
	{
		// TODO
		RBXAssert(0);
	}
}
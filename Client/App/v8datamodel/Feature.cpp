#include "v8datamodel/Feature.h"
#include "v8world/MotorJoint.h"

namespace RBX {
    // Note to whoever comes here next:
    // a lot of VelocityMotor's code is apparently just copied
    // from Motor in JointInstance.cpp

    const char* sFeature = "Feature";
    const char* sHole = "Hole";
    const char* sVelocityMotor = "VelocityMotor";

	float VelocityMotor::getMaxVelocity() const
	{
		return rbx_static_cast<MotorJoint*>(joint)->maxVelocity;
	}

	float VelocityMotor::getDesiredAngle() const
	{
		return rbx_static_cast<MotorJoint*>(joint)->desiredAngle;
	}

	float VelocityMotor::getCurrentAngle() const
	{
		return rbx_static_cast<MotorJoint*>(joint)->getCurrentAngle();
	}
}

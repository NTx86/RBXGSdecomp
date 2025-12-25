#include "v8datamodel/JointInstance.h"
#include "v8world/Primitive.h"
#include "util/Debug.h"

namespace RBX 
{
	const char *sJointInstance = "JointInstance";

	JointInstance::JointInstance(Joint* joint) 
		: joint(joint)
	{
		joint->setJointOwner(this);
	}

	// NEEDS: ~Instance
	JointInstance::~JointInstance()
	{
		RBXASSERT(getParent() == NULL);
		RBXASSERT(joint->getWorld() == NULL);

		joint->setJointOwner(NULL);
		delete joint;
		joint = NULL;
	}

	void JointInstance::render3dAdorn(Adorn* adorn) 
	{
		RBXASSERT(shouldRender3dAdorn());

		if (Joint::isAssemblyJoint(this->joint)) 
		{
			G3D::Color3 color3;

			Primitive* prim0 = this->joint->getPrimitive(0);
			Primitive* prim1 = this->joint->getPrimitive(1);

			G3D::Vector3 vec0 = prim0 
				? prim0->getCoordinateFrame().translation 
				: G3D::Vector3::zero();

			G3D::Vector3 vec1 = prim1 
				? prim1->getCoordinateFrame().translation 
				: G3D::Vector3::zero();

			if (this->joint->getActive())
				color3 = G3D::Color3::green();
			else
				color3 = G3D::Color3::red();

			adorn->setObjectToWorldMatrix(G3D::CoordinateFrame());

			G3D::Color4 color4(color3);
			adorn->lineSegment(
				G3D::LineSegment::fromTwoPoints(vec0, vec1), 
				color4, 
				20.0);
		} 
	}
}

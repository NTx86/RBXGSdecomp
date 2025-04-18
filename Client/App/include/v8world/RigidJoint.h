#pragma once
#include "v8world/Joint.h"

namespace RBX
{
	class RigidJoint : public Joint
	{
	private:
		virtual Joint::JointType getJointType() const;
		virtual bool isBroken() const;
	public:
		//RigidJoint(const RigidJoint&);
		RigidJoint(Primitive*, Primitive*, const G3D::CoordinateFrame&, const G3D::CoordinateFrame&);
		RigidJoint();
		virtual ~RigidJoint();
	public:
		virtual bool isAligned();
		virtual G3D::CoordinateFrame align(Primitive*, Primitive*);
		G3D::CoordinateFrame getChildInParent(Primitive*, Primitive*);
		//RigidJoint& operator=(const RigidJoint&);
  
	private:
		static bool jointIsRigid(Joint*);
	protected:
		static void faceIdToCoords(Primitive*, Primitive*, NormalId, NormalId, G3D::CoordinateFrame&, G3D::CoordinateFrame&);
	public:
		static bool isRigidJoint(Edge*);
	};
}

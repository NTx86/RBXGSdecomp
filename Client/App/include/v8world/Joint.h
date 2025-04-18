#pragma once
#include <G3DAll.h>
#include "v8world/Edge.h"
#include "util/NormalId.h"

namespace RBX
{
	class IJointOwner
	{
	};

	class Joint : public Edge
	{
	public:
		enum JointType
		{
			NO_JOINT,
			ROTATE_JOINT,
			ROTATE_P_JOINT,
			ROTATE_V_JOINT,
			GLUE_JOINT,
			ANCHOR_JOINT,
			WELD_JOINT,
			SNAP_JOINT,
			MOTOR_JOINT,
			FREE_JOINT
		};

	private:
		IJointOwner* jointOwner;
	protected:
		bool active;
		CoordinateFrame jointCoord0;
		CoordinateFrame jointCoord1;

	protected:
		virtual Edge::EdgeType getEdgeType();
	public:
		//Joint(const Joint&);
	protected:
		Joint(Primitive*, Primitive*, const G3D::CoordinateFrame&, const G3D::CoordinateFrame&);
		Joint();
	public:
		virtual ~Joint();
	public:
		void setJointOwner(RBX::IJointOwner*);
		IJointOwner* getJointOwner() const;
		virtual void setPrimitive(int, Primitive*);
		virtual JointType getJointType() const;
		virtual bool isBreakable() const;
		virtual bool isBroken() const;
		virtual bool joinsFace(Primitive*, NormalId) const;
		virtual bool isAligned();
		virtual G3D::CoordinateFrame align(Primitive*, Primitive*);
		virtual bool canStepUi() const;
		virtual void stepUi(int);
		bool getActive() const;
		void setActive(bool);
		void setJointCoord(int, const G3D::CoordinateFrame&);
		const G3D::CoordinateFrame& getJointCoord(int) const;
		NormalId getNormalId(int) const;
		//Joint& operator=(const Joint&);
  
	private:
		static bool canBuildJoint(Primitive*, Primitive*, NormalId, NormalId, float, float);
	protected:
		static bool canBuildJointLoose(Primitive*, Primitive*, NormalId, NormalId);
		static bool canBuildJointTight(Primitive*, Primitive*, NormalId, NormalId);
	public:
		static JointType getJointType(Edge*);
		static bool isJoint(Edge*);
		static bool isAssemblyJoint(Edge*);
		static bool isGroundJoint(Joint*);
		static bool isAutoJoinJoint(Joint*);
	};
}

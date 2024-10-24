#pragma once
#include <G3DAll.h>
#include "indexarray.h"
#include "simbody.h"
#include "link.h"
#include "pv.h"
#include "link.h"
#include "kernelindex.h"
#include "cofm.h"

namespace RBX {
	//class SimBody;
	class Body : KernelIndex
	{
		private:
			int& getIndex() {return index;};
			Body *root;
			Body *parent;
			int index;
			IndexArray<RBX::Body, &getIndex> children;
			Cofm *cofm;
			SimBody *simBody;
			bool canThrottle;
			RBX::Link *link;
			CoordinateFrame meInParent;
			Matrix3 moment;
			float mass;
			int stateIndex;
			PV pv;
			//int& getKernelIndex();
			const RBX::SimBody* getRootSimBody() {return getRoot()->simBody;};
			void resetRoot(RBX::Body* newRoot);
			bool validateParentCofmDirty();
			const G3D::CoordinateFrame& getMeInParent() const;
			void updatePV();
			void onChildAdded(RBX::Body* child);
			void onChildRemoved(RBX::Body* child);
			const RBX::Body* calcRootConst() const;
			__declspec(noinline) Body* Body::calcRoot()
			{
				return (getParent() ? getParent()->calcRoot() : this);
			}
		public:
			int& getKernelIndex() {return kernelIndex;}
			Body();
			~Body();
			void step(float, bool);
			bool cofmIsClean();
			void makeCofmDirty();
			void advanceStateIndex();
			void makeStateDirty();
			int getStateIndex() const;
			int numChildren() {return children.size();}
			RBX::Body* getChild(int) const;
			inline RBX::Body* getParent() const
			{
				return parent;
			}
			RBX::Link* getLink() const;
			const RBX::Body* getRootConst() const;
			RBX::Body* getRoot() {return root;};
			const G3D::Vector3& getCofmOffset() const
			{
				if (cofm)
				{
					return cofm->getCofmInBody();
				}
				else
				{
					return Vector3::zero();
				}
			}

			G3D::CoordinateFrame getMeInDescendant(const RBX::Body*) const;
			G3D::CoordinateFrame getMeInRoot() const;
			float getMass() const;
			G3D::Matrix3 getIBody() const;
			G3D::Vector3 getIBodyV3() const;
			G3D::Matrix3 getIWorld() const;
			G3D::Matrix3 getIWorldAtPoint(const G3D::Vector3&) const;
			float getBranchMass() const;
			G3D::Matrix3 getBranchIBody() const;
			G3D::Vector3 getBranchIBodyV3() const;
			G3D::Matrix3 getBranchIWorld() const;
			G3D::Matrix3 getBranchIWorldAtPoint(const G3D::Vector3&) const;
			G3D::Vector3 getBranchCofmPos() const;
			G3D::CoordinateFrame getBranchCofmCoordinateFrame() const;
			const G3D::Vector3& getPos() const;
			const G3D::CoordinateFrame& getCoordinateFrame() const;
			const RBX::Velocity& getVelocity() const;
			const RBX::PV& getPV() const;
			const bool getCanThrottle() const;
			void accumulateForceAtCofm(const G3D::Vector3&);
			void accumulateForceAtBranchCofm(const G3D::Vector3&);
			void accumulateForce(const G3D::Vector3&, const G3D::Vector3&);
			void accumulateTorque(const G3D::Vector3&);
			void resetAccumulators();
			const G3D::Vector3& getBranchForce() const;
			const G3D::Vector3& getBranchTorque() const;
			void setParent(RBX::Body*);
			void setMeInParent(RBX::Link*);
			void setMeInParent(const G3D::CoordinateFrame&);
			void setMass(float _mass);
			void setMoment(const G3D::Matrix3&);
			void setPv(const RBX::PV&);
			void setCoordinateFrame(const G3D::CoordinateFrame&);
			void setVelocity(const RBX::Velocity&);
			void setCanThrottle(bool);
			float kineticEnergy() const;
			float potentialEnergy() const;
	};
}
#pragma once
#include <G3DAll.h>
#include "quaternion.h"
#include "pv.h"
//#include "body.h"

namespace RBX {
	class Body;

	class SimBody
	{
		private:
		  void clearAccumulators();
		  void update();
		  void updateIfDirty()
		  {
			if (dirty)
				update();
		  }
		public:
			Body *body;
			bool dirty;
			PV pv;
			Quaternion qOrientation;
			G3D::Vector3 angMomentum;
			G3D::Vector3 momentRecip;
			float massRecip;
			float constantForceY;
			G3D::Vector3 force;
			G3D::Vector3 torque;
			//funcs
			//void SimBody(const RBX::SimBody&);
			SimBody(RBX::Body* body);
			~SimBody();
			void step(float);
			void makeDirty();
			bool getDirty() const;
			PV getOwnerPV();
			const PV& getPV();
			void accumulateForceCofm(const G3D::Vector3&);
			inline static G3D::Vector3 accumulateInline(const G3D::Vector3& _force, const G3D::Vector3& pvWorld, const G3D::Vector3& worldPos)
			{
				G3D::Vector3 temp = worldPos - pvWorld;
				return temp.cross(_force);
			}
			void accumulateForce(const G3D::Vector3& _force, const G3D::Vector3& worldPos)
			{
				updateIfDirty();
				force += _force;
				torque += accumulateInline(_force, pv.position.translation, worldPos);
			}
			void accumulateTorque(const G3D::Vector3&);
			void SimBody::resetAccumulators()
			{
				updateIfDirty();
				force = Vector3(0.0f, constantForceY, 0.0f);
				torque = Vector3(0.0f, 0.0f, 0.0f);
			}
			const G3D::Vector3& getForce() const;
			const G3D::Vector3& getTorque() const;
			void matchDummy();
	};
}
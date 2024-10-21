#pragma once

#include <G3DAll.h>
#include "velocity.h"

namespace RBX {
	class PV
	{
		public:
			G3D::CoordinateFrame position;
			RBX::Velocity velocity;
			PV pvAtLocalOffset(G3D::Vector3& localOffset) const;
			//PV& operator=(PV& setPV)
			//{
			//	position = setPV.position;
			//	velocity = setPV.velocity;
			//	return *this;
			//}
	};
}
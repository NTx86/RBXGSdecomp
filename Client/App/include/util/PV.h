#pragma once

#include <G3DAll.h>
#include "util/Velocity.h"

namespace RBX {
	class PV
	{
		public:
			G3D::CoordinateFrame position;
			RBX::Velocity velocity;
			PV pvAtLocalOffset(G3D::Vector3& localOffset) const;
			PV pvAtLocalCoord(const G3D::CoordinateFrame& localCoord) const;
			PV() {}
			PV(const G3D::CoordinateFrame& worldPos, const RBX::Velocity& _velocity):position(worldPos),velocity(_velocity) {}
	};
}
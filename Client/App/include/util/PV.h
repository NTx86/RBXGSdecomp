#pragma once

#include <G3DAll.h>
#include "util/Velocity.h"

namespace RBX {
	class PV
	{
		public:
			G3D::CoordinateFrame position;
			RBX::Velocity velocity;
			bool operator==(const PV&) const;
			bool operator!=(const PV&) const;
			PV(const PV&) {};
			PV(const G3D::CoordinateFrame& worldPos, const Velocity& _velocity):position(worldPos),velocity(_velocity) {};
			PV() {};
			~PV() {};
			PV inverse() const;
			PV toObjectSpace(const PV&) const;
			G3D::Vector3 linearVelocityAtPoint(const G3D::Vector3&) const;
			Velocity velocityAtPoint(const G3D::Vector3&) const;
			Velocity velocityAtLocalOffset(const G3D::Vector3&) const;
			PV pvAtLocalOffset(const G3D::Vector3& localOffset) const;
			PV pvAtLocalCoord(const G3D::CoordinateFrame& localCoord) const;
			PV operator*(const PV&) const;
	};
}
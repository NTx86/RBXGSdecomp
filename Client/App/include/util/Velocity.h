#pragma once
#include <G3DAll.h>

namespace RBX {
	class Velocity
	{
		public:
			G3D::Vector3 linear;
			G3D::Vector3 rotational;
			Velocity::Velocity():linear(Vector3::zero()),rotational(Vector3::zero())
			{}
	};
}
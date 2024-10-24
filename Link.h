#pragma once
#include <G3DAll.h>
//#include "Body.h"

namespace RBX
{
	class Body;
	class Link
	{
		public:
			RBX::Body *body;
			G3D::CoordinateFrame parentCoord;
			G3D::CoordinateFrame childCoord;
			G3D::CoordinateFrame childCoordInverse;
			G3D::CoordinateFrame childInParent;
			int stateIndex;
	};
}
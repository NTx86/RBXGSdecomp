#pragma once
#include <G3DAll.h>
//#include "body.h"

namespace RBX {
	class Body;
	class Cofm
	{
		public:
			Body *body;
			bool dirty;
			G3D::Vector3 cofmInBody;
			float mass;
			G3D::Matrix3 moment;
			G3D::Vector3& getCofmInBody() const;
			void updateIfDirty();
	};
}
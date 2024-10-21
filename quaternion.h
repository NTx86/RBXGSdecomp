#pragma once

namespace RBX {
	class Quaternion
	{
		public:
			float x;
			float y;
			float z;
			float w;
			Quaternion::Quaternion():x(0.0f),y(0.0f),z(0.0f),w(1.0f) {}
	};
}
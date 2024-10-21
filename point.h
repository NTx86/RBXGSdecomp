#pragma once
#include <G3DAll.h>
#include "body.h"

namespace RBX {
	class Point : RBX::KernelIndex
	{
		public:
			Point::Point(Body& _body);
			Point::Point(Body* _body); //i dont know enough about c++ to know why, this is supposed to be under protected
			virtual ~Point()
			{
				
			}
			__declspec(noinline) int& getKernelIndex() {return kernelIndex;}
			void step();
			void forceToBody();
			void setWorldPos(const G3D::Vector3& worldPos);
			int numOwners;
			RBX::Body *body;
			G3D::Vector3 localPos;
			G3D::Vector3 worldPos;
			G3D::Vector3 force;

			static bool Point::sameBodyAndOffset(const RBX::Point& p1, const RBX::Point& p2)
			{
				return (p1.body == p2.body
					&& p1.localPos.x == p2.localPos.x
					&& p1.localPos.y == p2.localPos.y
					&& p1.localPos.z == p2.localPos.z);
			}
	};
}
#pragma once
#include <G3DAll.h>
//#include "v8kernel/Body.h"

namespace RBX
{
	class Body;
	class Link
	{
		protected:
			RBX::Body *body;
			G3D::CoordinateFrame parentCoord;
			G3D::CoordinateFrame childCoord;
			G3D::CoordinateFrame childCoordInverse;
			G3D::CoordinateFrame childInParent;
			int stateIndex;
			virtual void computeChildInParent(const G3D::CoordinateFrame&) const;
			void dirty();
			//void setBody(RBX::Body* _body){ body = _body;}
		public:
			Link();
			~Link();
			const G3D::CoordinateFrame& getChildInParent();
			Body* getBody() const;
			void reset(const G3D::CoordinateFrame&, const G3D::CoordinateFrame&);
			void setBody(RBX::Body* _body){ body = _body;} // this isn't suppose to be in public but then i can't access it in Body::setParent()
	};

	class RevoluteLink
	{
	private:
		float jointAngle;
  
	private:
		virtual void computeChildInParent(G3D::CoordinateFrame&) const;
	public:
		//RevoluteLink(const RevoluteLink&);
		RevoluteLink();
	public:
		void setJointAngle(float);
	public:
		~RevoluteLink();
		//RBX::RevoluteLink& operator=(const RevoluteLink&);
	};
}
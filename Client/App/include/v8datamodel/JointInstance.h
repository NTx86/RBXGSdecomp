#pragma once
#include "v8world/Joint.h"
#include "v8tree/Instance.h"
#include "util/IRenderable.h"
#include "reflection/object.h"

namespace RBX {
    extern const char* sJointInstance;

	class JointInstance 
		: public DescribedNonCreatable<JointInstance, Instance, &sJointInstance>
		, public IRenderable, public IJointOwner {
	protected:
		RBX::Joint* joint;
		virtual bool shouldRender3dAdorn() const {
			return true;
		}

		virtual void render3dAdorn(Adorn* adorn);

	public:
		JointInstance(Joint* joint);
		~JointInstance();
    };
}
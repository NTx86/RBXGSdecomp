#include "v8world/Contact.h"

namespace RBX
{
	Contact::Contact(Primitive* prim0, Primitive* prim1)
		: Edge(prim0, prim1),
		jointK(0),
		elasticJointK(0),
		lastContactStep(-1),
		steppingIndex(-1),
		kFriction(0)
	{
	}

	void Contact::putInKernel(Kernel* _kernel)
	{
		IPipelined::putInKernel(_kernel);
		onPrimitiveContactParametersChanged();
	}

	void Contact::removeFromKernel()
	{
		RBXASSERT(IPipelined::getKernel());

		deleteAllConnectors();
		IPipelined::removeFromKernel();
	}

	Contact::~Contact()
	{
	}
}
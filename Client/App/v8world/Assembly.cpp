#include "v8world/Assembly.h"
#include "v8world/Clump.h"

namespace RBX
{
	Primitive* Assembly::getMainPrimitive() const
	{
		return rootClump->getRootPrimitive();
	}
}
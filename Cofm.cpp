#include "Cofm.h"

using namespace RBX;

//haven't checked if it matches yet
Cofm::Cofm(Body* body):body(body)
					,dirty(1)
					,cofmInBody(Vector3(0.0f,0.0f,0.0f))
					{}

void Cofm::updateIfDirty()
{
	return; //placeholder
}
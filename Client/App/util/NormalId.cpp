#include "util/NormalId.h"

namespace RBX
{
	// TODO: check if this matches
	bool validNormalId(NormalId id)
	{
		return id <= NORM_Z_NEG;
	}
}
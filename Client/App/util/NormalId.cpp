#include "util/NormalId.h"

namespace RBX
{
	bool validNormalId(NormalId id)
	{
		return id >= NORM_X && id <= NORM_Z_NEG;
	}
}
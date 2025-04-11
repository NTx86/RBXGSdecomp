#pragma once

namespace RBX
{
	enum NormalId
	{
		NORM_X = 0,
		NORM_Y = 1,
		NORM_Z = 2,
		NORM_X_NEG = 3,
		NORM_Y_NEG = 4,
		NORM_Z_NEG = 5,
		NORM_UNDEFINED = 6,
	};

	bool validNormalId(NormalId normalId);
}
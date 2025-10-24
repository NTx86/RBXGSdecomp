#pragma once

#include <G3DAll.h>
#include "util/SurfaceType.h"

namespace RBX
{
	class Part
	{
	public:
		enum PartType
		{
			BALL_PART,
			BLOCK_PART,
			CYLINDER_PART,
		};
	public:
		PartType type;
		Vector3 gridSize;
		Color4 color;
		Vector6<SurfaceType> surfaceType;
		CoordinateFrame coordinateFrame; 
		
		Part(const Part &);
		Part(PartType, const Vector3&, const Color4, const Vector6<SurfaceType>&, const CoordinateFrame&);
		Part(PartType, const Vector3&, const Color4, const CoordinateFrame&);
		~Part();
		Part& operator=(const Part&);
	};
}

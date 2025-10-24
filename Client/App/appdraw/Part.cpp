#include "appdraw/Part.h"

namespace RBX
{
	Part::Part(PartType type, const Vector3& gridSize, const Color4 color, const Vector6<SurfaceType>& surfaceType, const CoordinateFrame& c)
		: type(type),
		  gridSize(gridSize),
		  color(color),
		  surfaceType(surfaceType),
		  coordinateFrame(c)	  
	{
	}

	Part::Part(PartType type, const Vector3& gridSize, const Color4 color, const CoordinateFrame& c)
		: type(type),
		  gridSize(gridSize),
		  color(color),
		  surfaceType(),
		  coordinateFrame(c)
	{
	}

	Part::Part(const Part & __that)
		: type(__that.type),
		  gridSize(__that.gridSize),
		  color(__that.color),
		  surfaceType(__that.surfaceType),
		  coordinateFrame(__that.coordinateFrame)
	{
	}

	Part& Part::operator=(const Part& __that) 
	{
		this->type = __that.type;
		this->gridSize = __that.gridSize;
		this->color = __that.color;
		this->surfaceType = __that.surfaceType;
		this->coordinateFrame = __that.coordinateFrame;
		return *this;
	}
}
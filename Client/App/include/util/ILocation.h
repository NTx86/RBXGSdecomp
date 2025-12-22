#pragma once
#include <g3d/coordinateframe.h>

namespace RBX
{
	class ILocation
	{
	public:
		virtual const G3D::CoordinateFrame getLocation() const = 0;
	public:
		//ILocation(const ILocation&);
		ILocation();
	public:
		//ILocation& operator=(const ILocation&);
	};
}

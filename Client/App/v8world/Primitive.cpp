#include "v8world/Primitive.h"

namespace RBX
{
	//100% Match
	Edge* Primitive::getFirstEdge() const
	{
		Edge *edge;

		edge = joints.first;
		if (!edge)
			edge = contacts.first;

		return edge;
	}

	//100% Match
	void Primitive::setClump(Clump *clump)
	{
		if (clump != this->clump)
			this->clump = clump;
	}
	
	//100% Match
	float Primitive::computeJointK() const
	{
		Geometry::GeometryType type;
		
		type = geometry->getGeometryType();
		return RBX::Constants::getJointK(geometry->getGridSize(), 
			type == geometry->GEOMETRY_BALL);
	}

	//60% Match
	float Primitive::getPlanarSize() const
	{
		float fVar1;
		Geometry *geometry;
		const Vector3 *gridSize;

		geometry = this->geometry;
		gridSize = &geometry->getGridSize();
		fVar1 = (geometry->getGridSize()).z;
		if ((geometry->getGridSize()).y <= gridSize->x) 
		{
			if ((geometry->getGridSize()).y < fVar1) 
			{
				return (geometry->getGridSize()).z * gridSize->x;
			}
		}
		else if (gridSize->x < fVar1) 
		{
			return (geometry->getGridSize()).z * (geometry->getGridSize()).y;
		}
		return gridSize->x * (geometry->getGridSize()).y;
	}
}

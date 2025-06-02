#include "v8world/Primitive.h"

namespace RBX
{
	//100% Match
	void Primitive::setGuid(const RBX::Guid &value)
	{
		RBXASSERT(!world);
		guid.assign(value.getData());
		guidSetExternally = 1;
	}

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
	Assembly *Primitive::getAssembly() const
	{
		if (clump)
			return clump->getAssembly();

		return NULL;
	}
	
	//100% Match
	float Primitive::computeJointK() const
	{
		Geometry::GeometryType type;
		
		type = geometry->getGeometryType();
		return RBX::Constants::getJointK(geometry->getGridSize(), 
			type == geometry->GEOMETRY_BALL);
	}
	
	//100% Match
	void Primitive::setVelocity(const Velocity &vel)
	{
		body->setVelocity(vel);
	}

	//93% Match
	void Primitive::setSurfaceData(NormalId id, const SurfaceData &newSurfaceData)
	{
		SurfaceData *pSVar2;

		if (!surfaceData[id] && newSurfaceData.isEmpty())
			return;

		pSVar2 = surfaceData[id];
		if (!pSVar2 || pSVar2->inputType != newSurfaceData.inputType 
			|| pSVar2->paramA != newSurfaceData.paramA 
			|| pSVar2->paramB != newSurfaceData.paramB)
		{
			if (newSurfaceData.isEmpty()) 
			{
				RBXASSERT(!surfaceData[id]);
				operator delete(surfaceData[id]);
				surfaceData[id] = NULL;
				return;
			}
			if (!this->surfaceData[id])
			{
				pSVar2 = (SurfaceData*) operator new(sizeof(SurfaceData));
				if (pSVar2)
				{
					pSVar2->inputType = Controller::NO_INPUT;
					pSVar2->paramA = -0.5;
					pSVar2->paramB = 0.5;
				}
				else
				{
					pSVar2 = NULL;
				}
				surfaceData[id] = pSVar2;
			}
			*surfaceData[id] = newSurfaceData;
		}
	}

	//100% Match
	void Primitive::setSurfaceType(NormalId id, SurfaceType newSurfaceType)
	{
		if (this->surfaceType[id] != newSurfaceType)
			this->surfaceType[id] = newSurfaceType;
	}

	//88% Match
	Joint* Primitive::getFirstJoint() const
	{
		Edge *first;
		first = (this->joints).first;
		
		RBXASSERT(dynamic_cast<Joint*>(first) == first);
		return (Joint*)first;
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

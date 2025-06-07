#include "v8world/World.h"
#include "v8world/Joint.h"
#include "v8world/Clump.h"
#include "v8world/Contact.h"
#include "v8world/RigidJoint.h"
#include "v8world/Anchor.h"
#include "v8world/Assembly.h"
#include "v8world/IMoving.h"
#include "v8world/Primitive.h"

namespace RBX
{

	Extents Primitive::getExtentsWorld() const
	{
		Geometry *geometry = this->geometry;
		Extents extent(-(geometry->getGridSize() * 0.5), geometry->getGridSize() * 0.5);
		const CoordinateFrame &frame = getCoordinateFrame();

		return extent.toWorldSpace(frame);
	}

	void Primitive::setGuid(const RBX::Guid &value)
	{
		RBXASSERT(!world);
		guid.assign(value.getData());
		guidSetExternally = 1;
	}

	Edge* Primitive::getFirstEdge() const
	{
		Edge *edge = joints.first;
		if (!edge)
			edge = contacts.first;

		return edge;
	}

	Edge* Primitive::getNextEdge(Edge* e) const
	{
		Edge *result = e->getNext(this);
		if (!result)
		{
			if (e->getEdgeType())
				return NULL;
			else
				return this->contacts.first;
		}
		return result;
	}

	void Primitive::setClump(Clump *clump)
	{
		if (clump != this->clump)
			this->clump = clump;
	}

	Assembly *Primitive::getAssembly() const
	{
		if (clump)
			return clump->getAssembly();

		return NULL;
	}
	
	float Primitive::computeJointK() const
	{	
		Geometry::GeometryType type = geometry->getGeometryType();
		return RBX::Constants::getJointK(geometry->getGridSize(), 
			type == geometry->GEOMETRY_BALL);
	}

	void Primitive::setCanSleep(bool canSleep)
	{
		if (canSleep != this->canSleep)
		{
			this->canSleep = canSleep;
			if (world)
			{
				world->onPrimitiveCanSleepChanged(this);
			}	
		}
	}

	void Primitive::setCanCollide(bool canCollide)
	{
		bool cVar2 = !dragging && this->canCollide;

		if (this->canCollide != canCollide) 
		{
			this->canCollide = canCollide;
			if (world) 
			{
				bool cVar1 = !dragging && canCollide;
				if (cVar2 != cVar1) 
				{
					world->onPrimitiveCanCollideChanged(this);
				}
			}
		}
	}

	void Primitive::setFriction(float friction)
	{
		if (friction != this->friction) 
		{
			this->friction = friction;
			if (world) 
			{
				world->onPrimitiveContactParametersChanged(this);
			}
		}
	}

	void Primitive::setElasticity(float elasticity)
	{
		if (elasticity != this->elasticity) 
		{
			this->elasticity = elasticity;
			if (world) 
			{
				world->onPrimitiveContactParametersChanged(this);
			}
		}
	}
	
	void Primitive::setVelocity(const Velocity &vel)
	{
		body->setVelocity(vel);
	}

	void Primitive::setSurfaceData(NormalId id, const SurfaceData &newSurfaceData)
	{
		if (!surfaceData[id] && newSurfaceData.isEmpty())
			return;

		SurfaceData *pSVar2 = surfaceData[id];
		if (!pSVar2 || pSVar2->inputType != newSurfaceData.inputType 
			|| pSVar2->paramA != newSurfaceData.paramA 
			|| pSVar2->paramB != newSurfaceData.paramB)
		{
			if (newSurfaceData.isEmpty()) 
			{
				RBXASSERT(surfaceData[id]);
				operator delete(surfaceData[id]);
				surfaceData[id] = NULL;
				return;
			}
			if (!this->surfaceData[id])
			{
				pSVar2 = new SurfaceData();
				if (pSVar2)
				{
					pSVar2->inputType = Controller::NO_INPUT;
					pSVar2->paramA = -0.5f;
					pSVar2->paramB = 0.5f;
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

	void Primitive::setSurfaceType(NormalId id, SurfaceType newSurfaceType)
	{
		if (this->surfaceType[id] != newSurfaceType)
			this->surfaceType[id] = newSurfaceType;
	}

	Joint* Primitive::getFirstJoint() const
	{
		Edge *first = this->joints.first;
		return rbx_static_cast<Joint*>(first);
	}

	Joint* Primitive::getNextJoint(Joint *prev) const
	{
		Edge *pJVar2 = prev->getNext(this);
		return rbx_static_cast<Joint*>(pJVar2);
	}

	Contact* Primitive::getFirstContact()
	{
		Edge *pCVar1 = this->contacts.first;
		return rbx_static_cast<Contact*>(pCVar1);
	}

	Contact* Primitive::getNextContact(Contact *prev)
	{
		Edge *pCVar1 = prev->getNext(this);
		return rbx_static_cast<Contact*>(pCVar1);
	}

	RigidJoint* Primitive::getFirstRigid()
	{
		Edge *pEVar1 = this->joints.first;
		if (!pEVar1) 
			pEVar1 = this->contacts.first;
		RigidJoint *pRVar2 = getFirstRigidAt(pEVar1);
		return pRVar2;
	}
	
	/*
	RigidJoint* Primitive::getNextRigid(RigidJoint *prev)
	{
		Edge *next = prev->getNext(this);

		if (!next)
		{
			if (!prev->getEdgeType())
			{
				return getFirstRigidAt(contacts.first);
			}
			next = NULL;
		}
		return getFirstRigidAt(next);
	}
	*/

	float Primitive::getRadius() const
	{
		return this->geometry->getRadius();
	}

	void Primitive::setAnchor(bool anchor)
	{
		Anchor *anchorObject = this->anchorObject;
		this->anchored = anchor;

		if ( anchor || this->dragging )
			anchor = true;

		if ( anchor != (anchorObject != NULL) )
		{
			if (anchor)
			{
				RBXASSERT(!anchorObject);
				anchorObject = new Anchor(this);
				this->anchorObject = anchorObject;
				if (this->world)
					this->world->onPrimitiveAddedAnchor(this);
			}
			else
			{
				RBXASSERT(anchorObject);
				if (world)
					this->world->onPrimitiveRemovingAnchor(this);
				if (this->anchorObject)
					operator delete(this->anchorObject);
				this->anchorObject = NULL;
			}
		}
	}

	void Primitive::setCoordinateFrame(const CoordinateFrame &cFrame)
	{
		Body *body = this->body;
		bool bVar1 = cFrame.operator != (body->getPV().position);
		if (bVar1) 
		{
			Assembly *this_01 = (this->clump) ? this->clump->getAssembly() : NULL;

			if (!this_01) 
			{
				this->body->setCoordinateFrame(cFrame);
				this->myOwner->notifyMoved();
				if (this->world) 
				{
					this->world->onPrimitiveExtentsChanged(this);
				}
			}
			else 
			{
				RBXASSERT(this->world);
				if (this_01->getMainPrimitive() == this) 
				{
					this->body->setCoordinateFrame(cFrame);
					this_01->notifyMoved();
					this->world->onAssemblyExtentsChanged(this_01);
					if (this->anchored == false) 
					{
						this->world->ticklePrimitive(this);
					}
				}
			}
		}
	}

	void Primitive::setDragging(bool dragging)
	{
		if (this->dragging != dragging) 
		{
			bool cVar2 = this->dragging == false && this->canCollide != false;

			this->dragging = dragging;
			this->setAnchor(this->anchored);
			if (world)
			{
				bool cVar1 = this->dragging == false && this->canCollide != false;
				if (cVar1 != cVar2) 
				{
					this->world->onPrimitiveCanCollideChanged(this);
				}
			}
		}
	}

	float Primitive::getPlanarSize() const
	{
		Geometry *geometry = this->geometry;
		const Vector3 *gridSize = &geometry->getGridSize();
		float fVar1 = (geometry->getGridSize()).z;
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

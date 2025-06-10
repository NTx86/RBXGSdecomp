#include "v8world/World.h"
#include "v8world/Joint.h"
#include "v8world/Clump.h"
#include "v8world/Contact.h"
#include "v8world/RigidJoint.h"
#include "v8world/Anchor.h"
#include "v8world/Assembly.h"
#include "v8world/IMoving.h"
#include "v8world/Ball.h"
#include "v8world/Block.h"
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
			if (!e->getEdgeType())
				return this->contacts.first;
			else
				return NULL;
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

	CoordinateFrame Primitive::getFaceCoordInObject(NormalId objectFace)
	{
		Geometry *pGVar7 = this->geometry;
		const Vector3 *pVVar8 = &normalIdToVector3(objectFace);

		return CoordinateFrame(Matrix3(normalIdToMatrix3(objectFace)), Vector3(pVVar8->x * pGVar7->getGridSize().x,
			pVVar8->y * pGVar7->getGridSize().y, pVVar8->z * pGVar7->getGridSize().z));
	}

	Face Primitive::getFaceInObject(NormalId objectFace)
	{
		Geometry *geometry = this->geometry;
		Extents extent(-(geometry->getGridSize() * 0.5), geometry->getGridSize() * 0.5);
		return Face::fromExtentsSide(extent, objectFace);
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
				delete surfaceData[id];
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

	RigidJoint* Primitive::getFirstRigidAt(Edge *edge)
	{
		Edge *pRVar2 = edge;

		if (edge) 
		{
			while (true)
			{
				if (edge->getEdgeType() == Edge::JOINT)
				{
					pRVar2 = rbx_static_cast<Joint*>(edge);
					if ((edge->getEdgeType() == 6) || (edge->getEdgeType() == 7))
						break;
				}
				pRVar2 = edge->getNext(this);
				if (pRVar2) 
				{
					edge = (Edge *)pRVar2;
				}
				else
				{
					if (!edge->getEdgeType()) 
						return NULL;

					pRVar2 = (RigidJoint *)(this->contacts).first;
				}

			}
		}
		return rbx_static_cast<RigidJoint*>(pRVar2);
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

	Joint* Primitive::getJoint(Primitive *p0, Primitive *p1)
	{
		Primitive *pPVar4 = p0;
		if (p0->joints.num >= p1->joints.num)
			pPVar4 = p1;

		Joint *pJVar3 = rbx_static_cast<Joint*>(pPVar4->joints.first);
		while(true)
		{
			if (!pJVar3) 
				return NULL;

			Primitive *pPVar1 = pJVar3->getPrimitive(0);
			if (p0 == pPVar1 && p1 == pJVar3->getPrimitive(1)) break;
			if (p0 == pJVar3->getPrimitive(1) && p1 == pPVar1) break;

			pJVar3 = rbx_static_cast<Joint*>(pJVar3->getNext(pPVar4));
		}
		return pJVar3;
	}

	Contact* Primitive::getContact(Primitive *p0, Primitive *p1)
	{
		Primitive *pPVar4 = p0;
		if (p0->contacts.num >= p1->contacts.num)
			pPVar4 = p1;

		Contact *pJVar3 = rbx_static_cast<Contact*>(pPVar4->contacts.first);
		while(true)
		{
			if (!pJVar3) 
				return NULL;

			Primitive *pPVar1 = pJVar3->getPrimitive(0);
			if (p0 == pPVar1 && p1 == pJVar3->getPrimitive(1)) break;
			if (p0 == pJVar3->getPrimitive(1) && p1 == pPVar1) break;

			pJVar3 = rbx_static_cast<Contact*>(pJVar3->getNext(pPVar4));
		}
		return pJVar3;
	}

	Geometry* Primitive::newGeometry(Geometry::GeometryType geometryType)
	{
		if (geometryType == Geometry::GEOMETRY_BALL)
		{
			Geometry *pGVar1 = new Ball();
			if (pGVar1)
			{
				return pGVar1;
			}
		}
		else
		{
			if (geometryType != Geometry::GEOMETRY_BLOCK)
			{
				Geometry *pGVar1 = Geometry::nullGeometry();
				return pGVar1;
			}
			Geometry *pGVar1 = new Block();
			if (pGVar1)
			{
				return pGVar1;
			}
		}
		return NULL;
	}

	Primitive::~Primitive()
	{
		if (this->geometry->getGeometryType() != Geometry::GEOMETRY_NONE)
		{
			Geometry *geometry = this->geometry;
			if (geometry)
				geometry->~Geometry();
			Body *body = this->body;
			if (body)
			{
				body->~Body();
				delete(body);
			}
		}
		delete(this->surfaceData);

		RBXASSERT(this->world);
		RBXASSERT(!this->clump);
		RBXASSERT(this->joints.first);
		RBXASSERT(!this->joints.num);
		RBXASSERT(this->contacts.first);
		RBXASSERT(!this->contacts.num);
		//RBXASSERT(!this->currentStage);
	}

	float Primitive::getRadius() const
	{
		return this->geometry->getRadius();
	}

	bool Primitive::hitTest(const Ray &worldRay, Vector3 &worldHitPoint, bool &inside)
	{
		Body *pBVar1 = this->body;
		Vector3 localHitPoint(0, 0, 0); 

		bool cVar2 = this->geometry->hitTest(pBVar1->getPV().position.toObjectSpace(worldRay), localHitPoint, inside);
		if (cVar2)
		{
			pBVar1 = this->body;
			Vector3 worldHitPoint = pBVar1->getPV().position.pointToWorldSpace(localHitPoint);
			return true;
		}
		return false;
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
					delete this->anchorObject;
				this->anchorObject = NULL;
			}
		}
	}

	Face Primitive::getFaceInWorld(NormalId objectFace)
	{
		Body *body = this->body;
		return this->getFaceInObject(objectFace).toWorldSpace(body->getPV().position);
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

	const CoordinateFrame &Primitive::getCoordinateFrame() const
	{
		return body->getPV().position;
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

	void Primitive::setPrimitiveType(Geometry::GeometryType geometryType)
	{
		RBXASSERT(this->geometry->getGeometryType() != Geometry::GEOMETRY_NONE);
		RBXASSERT(geometryType != Geometry::GEOMETRY_NONE);

		Geometry::GeometryType GVar2 = this->geometry->getGeometryType();
		if (GVar2 != geometryType) 
		{
			Geometry *pGVar3 = this->geometry;
			const Vector3 VStack_c(pGVar3->getGridSize());
			if (pGVar3) 
				pGVar3->~Geometry();
			pGVar3 = newGeometry(geometryType);
			this->geometry = pGVar3;
			if (this->world) 
				this->world->onPrimitiveGeometryTypeChanged(this);
			this->setGridSize(VStack_c);
			this->JointK.setDirty();
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

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
		Extents extent(-(geometry->getGridSize() * 0.5), geometry->getGridSize() * 0.5);
		return extent.toWorldSpace(getCoordinateFrame());
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
		bool canCollide1 = !dragging && this->canCollide;

		if (this->canCollide != canCollide) 
		{
			this->canCollide = canCollide;
			if (world) 
			{
				bool canCollide2 = !dragging && canCollide;
				if (canCollide1 != canCollide2) 
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
		return CoordinateFrame(normalIdToMatrix3(objectFace), 
			normalIdToVector3(objectFace) * geometry->getGridSize());
	}

	Face Primitive::getFaceInObject(NormalId objectFace)
	{
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

		SurfaceData *data = surfaceData[id];
		if (!data || data->inputType != newSurfaceData.inputType 
			|| data->paramA != newSurfaceData.paramA 
			|| data->paramB != newSurfaceData.paramB)
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
				data = new SurfaceData();
				if (data)
				{
					data->inputType = Controller::NO_INPUT;
					data->paramA = -0.5f;
					data->paramB = 0.5f;
				}
				else
				{
					data = NULL;
				}
				surfaceData[id] = data;
			}
			*surfaceData[id] = newSurfaceData;
		}
	}

	void Primitive::setSurfaceType(NormalId id, SurfaceType newSurfaceType)
	{
		if (this->surfaceType[id] != newSurfaceType)
			this->surfaceType[id] = newSurfaceType;
	}

	Extents Primitive::computeFuzzyExtents() const
	{
		Vector3 centerToCornerVector = this->geometry->getCenterToCorner(body->getPV().position.rotation);

		return Extents((body->getPV().position.translation - centerToCornerVector) - Vector3(0.01f, 0.01f, 0.01f), 
			(body->getPV().position.translation + centerToCornerVector) + Vector3(0.01f, 0.01f, 0.01f));
	}

	void Primitive::removeEdge(Edge* e)
	{
		Primitive *prim0 = e->getPrimitive(0);
		Primitive *prim1 = e->getPrimitive(1);

		if (!e->getEdgeType())
		{
			EdgeList::removeEdge(prim0, e, prim0->joints);
			if (prim1)
			{
				EdgeList::removeEdge(prim1, e, prim1->joints);
			}
			e = rbx_static_cast<Joint*>(e);
			delete e;
		}
		else
		{
			EdgeList::removeEdge(prim0, e, prim0->contacts);
			EdgeList::removeEdge(prim1, e, prim1->contacts);
		}
	}
	
	Joint* Primitive::getFirstJoint() const
	{
		return rbx_static_cast<Joint*>(this->joints.first);
	}

	Joint* Primitive::getNextJoint(Joint *prev) const
	{
		return rbx_static_cast<Joint*>(prev->getNext(this));
	}

	Contact* Primitive::getFirstContact()
	{
		return rbx_static_cast<Contact*>(this->contacts.first);
	}

	Contact* Primitive::getNextContact(Contact *prev)
	{
		return rbx_static_cast<Contact*>(prev->getNext(this));
	}

	RigidJoint* Primitive::getFirstRigidAt(Edge *edge)
	{
		if (edge)
		{
			while (true)
			{
				if (edge->getEdgeType() == Edge::JOINT)
				{
					if(RigidJoint::isRigidJoint(edge))
						break;
				}
				edge = edge->getNext(this);
				if (!edge) 
				{
					if (edge->getEdgeType()) 
						return NULL;

					edge = this->contacts.first;
				}
			}
		}
		return rbx_static_cast<RigidJoint*>(edge);
	}

	RigidJoint* Primitive::getFirstRigid()
	{
		Edge *firstEdge = this->joints.first;
		if (!firstEdge) 
			firstEdge = this->contacts.first;
		RigidJoint *rigidJoint = getFirstRigidAt(firstEdge);
		return rigidJoint;
	}
	
	RigidJoint* Primitive::getNextRigid(RigidJoint *prev)
	{
		Edge *next = prev->getNext(this);

		if (!next)
		{
			if (!next->getEdgeType())
			{
				return getFirstRigidAt(contacts.first);
			}
			next = NULL;
		}
		return getFirstRigidAt(next);
	}

	Joint* Primitive::getJoint(Primitive *p0, Primitive *p1)
	{
		Primitive *prim = p0;
		if (p0->joints.num >= p1->joints.num)
			prim = p1;

		Joint *firstJoint = rbx_static_cast<Joint*>(prim->joints.first);
		while(true)
		{
			if (!firstJoint) 
				return NULL;

			Primitive *jointPrim0 = firstJoint->getPrimitive(0);
			if (p0 == jointPrim0 && p1 == firstJoint->getPrimitive(1)) break;
			if (p0 == firstJoint->getPrimitive(1) && p1 == jointPrim0) break;

			firstJoint = rbx_static_cast<Joint*>(firstJoint->getNext(prim));
		}
		return firstJoint;
	}

	Contact* Primitive::getContact(Primitive *p0, Primitive *p1)
	{
		Primitive *prim = p0;
		if (p0->contacts.num >= p1->contacts.num)
			prim = p1;

		Contact *firstContact = rbx_static_cast<Contact*>(prim->contacts.first);
		while(true)
		{
			if (!firstContact) 
				return NULL;

			Primitive *contactPrim0 = firstContact->getPrimitive(0);
			if (p0 == contactPrim0 && p1 == firstContact->getPrimitive(1)) break;
			if (p0 == firstContact->getPrimitive(1) && p1 == contactPrim0) break;

			firstContact = rbx_static_cast<Contact*>(firstContact->getNext(prim));
		}
		return firstContact;
	}

	Geometry* Primitive::newGeometry(Geometry::GeometryType geometryType)
	{
		if (geometryType == Geometry::GEOMETRY_BALL)
		{
			Geometry *object = new Ball();
			if (object)
				return object;
		}
		else
		{
			if (geometryType != Geometry::GEOMETRY_BLOCK)
			{
				Geometry *object = Geometry::nullGeometry();
				return object;
			}
			Geometry *object = new Block();
			if (object)
				return object;
		}
		return NULL;
	}

	Primitive::~Primitive()
	{
		if (geometry->getGeometryType() != Geometry::GEOMETRY_NONE)
		{
			if (geometry)
				delete geometry;
			if (body)
				delete body;
		}
		delete(this->surfaceData);

		RBXASSERT(this->world);
		RBXASSERT(this->clump == 0);
		RBXASSERT(this->joints.first);
		RBXASSERT(this->joints.num == 0);
		RBXASSERT(this->contacts.first);
		RBXASSERT(this->contacts.num == 0);
	}

	float Primitive::getRadius() const
	{
		return this->geometry->getRadius();
	}

	bool Primitive::hitTest(const Ray &worldRay, Vector3 &worldHitPoint, bool &inside)
	{
		Vector3 localHitPoint(0, 0, 0); 

		bool hit = this->geometry->hitTest(body->getPV().position.toObjectSpace(worldRay), localHitPoint, inside);
		if (hit)
		{
			Vector3 worldHitPoint = body->getPV().position.pointToWorldSpace(localHitPoint);
			return true;
		}
		return false;
	}

	void Primitive::setAnchor(bool anchor)
	{
		Anchor *anchorObject = this->anchorObject;
		bool isAnchorObject = anchorObject != NULL;
		this->anchored = anchor;

		if ( anchor || this->dragging )
			anchor = true;

		if (anchor != isAnchorObject)
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
		return this->getFaceInObject(objectFace).toWorldSpace(body->getPV().position);
	}

	void Primitive::setCoordinateFrame(const CoordinateFrame &cFrame)
	{
		bool ifcFrame = cFrame != body->getPV().position;
		if (ifcFrame) 
		{
			Assembly *assembly = (this->clump) ? this->clump->getAssembly() : NULL;

			if (!assembly) 
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
				if (assembly->getMainPrimitive() == this) 
				{
					this->body->setCoordinateFrame(cFrame);
					assembly->notifyMoved();
					this->world->onAssemblyExtentsChanged(assembly);
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

	CoordinateFrame Primitive::getGridCorner() const
	{
		return CoordinateFrame(body->getPV().position.rotation, 
			body->getPV().position.pointToWorldSpace(-(geometry->getGridSize() * 0.5f)));
	}

	int Primitive::countNumJoints() const
	{
		int numJoints = 0;
		Joint *joint = rbx_static_cast<Joint*>(this->joints.first);

		while (joint)
		{
			Joint::JointType jointType = joint->getJointType();
			if (jointType != Joint::FREE_JOINT && jointType != Joint::ANCHOR_JOINT) 
				numJoints++;
			joint = rbx_static_cast<Joint*>(joint->getNext(this));
		}
		return numJoints;
	}

	void Primitive::setGridSize(const Vector3 &gridSize)
	{
		Vector3 protectedSize = this->clipToSafeSize(gridSize);

		if (geometry->getGridSize().x != protectedSize.x 
			|| geometry->getGridSize().y != protectedSize.y 
			|| geometry->getGridSize().z != protectedSize.z) 
		{
			this->fuzzyExtentsStateId = -2;
			this->geometry->setGridSize(protectedSize);
			const float mass = this->geometry->getGridVolume();
			this->body->setMass(mass);
			const Matrix3 &moment = this->geometry->getMoment(mass);
			this->body->setMoment(moment);
			this->JointK.setDirty();
			if (this->world) 
			{
				this->world->onPrimitiveExtentsChanged(this);
			}
			this->JointK.setDirty();
		}
	}

	void Primitive::setDragging(bool dragging)
	{
		if (this->dragging != dragging) 
		{
			bool notDrag1 = !dragging && this->canCollide;

			this->dragging = dragging;
			this->setAnchor(this->anchored);
			if (world)
			{
				bool notDrag2 = !this->dragging && this->canCollide;
				if (notDrag2 != notDrag1)
				{
					this->world->onPrimitiveCanCollideChanged(this);
				}
			}
		}
	}

	void Primitive::setGridCorner(const CoordinateFrame &gridCorner)
	{
		this->setCoordinateFrame(CoordinateFrame(gridCorner.rotation, 
			gridCorner.pointToWorldSpace(geometry->getGridSize() * 0.5f)));
	}

	void Primitive::setController(Controller *controller)
	{
		if (!controller)
			controller = NullController::getStaticNullController();

		if (this->controller != controller) 
			this->controller = controller;
	}

	void Primitive::setPrimitiveType(Geometry::GeometryType geometryType)
	{
		RBXASSERT(geometry->getGeometryType() != Geometry::GEOMETRY_NONE);
		RBXASSERT(geometryType != Geometry::GEOMETRY_NONE);

		Geometry::GeometryType currentGeometryType = this->geometry->getGeometryType();
		if (currentGeometryType != geometryType) 
		{
			const Vector3 gridSize(geometry->getGridSize());
			if (geometry) 
				delete geometry;
			this->geometry = newGeometry(geometryType);
			if (this->world) 
				this->world->onPrimitiveGeometryTypeChanged(this);
			this->setGridSize(gridSize);
			this->JointK.setDirty();
		}
	}

	float Primitive::getPlanarSize() const
	{
		const Vector3 *gridSize = &geometry->getGridSize();

		if (gridSize->y <= gridSize->x) 
		{
			if (gridSize->y < gridSize->z)
			{
				return gridSize->z * gridSize->x;
			}
		}
		else if (gridSize->x < gridSize->z) 
		{
			return gridSize->z * gridSize->y;
		}
		return gridSize->x * gridSize->y;
	}
}

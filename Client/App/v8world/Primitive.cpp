#include "v8world/Primitive.h"
#include "v8world/Geometry.h"
#include "v8world/Ball.h"
#include "v8world/Block.h"
#include <cmath>

namespace RBX {

	Primitive::Primitive(Geometry::GeometryType geometryType) : // WIP
		guidSetExternally(false),
		world(NULL),
		clump(NULL),
		spatialNodes(NULL),
		worldIndex(-1),
		clumpDepth(-1),
		traverseId(-1),
		fuzzyExtentsStateId(-2),
		geometry(newGeometry(geometryType)),
		body(new Body),
		elasticity(0.75f),
		canCollide(true),
		canSleep(true),
		JointK(this, &Primitive::computeJointK)
	{ // TODO: declare NullController class in controller.h
		for(int i = 0; i < 6; i++) 
		{
			surfaceType[i] = SurfaceType::NO_SURFACE;
			surfaceData[i] = NULL;
		}
	}

	Primitive::~Primitive() // WIP
	{
		Geometry::GeometryType type = geometry->getGeometryType();
		if(type != Geometry::GeometryType::GEOMETRY_NONE) {
			if(geometry) 
				delete geometry;
			if(body)
				delete body;
		}
		for(int i = 0; i < 6; i++)
			delete surfaceData[i];

		RBXASSERT(!inPipeline());
		RBXASSERT(!clump && !world);
	}

	G3D::Vector3 Primitive::clipToSafeSize(const G3D::Vector3& newSize)
	{
		G3D::Vector3 r = newSize.min(Vector3(512.0f, 512.0f, 512.0f));

		if(r.x * r.y * r.z > 1e+6f) {
			r.y = floorf(1e+6f / (r.x * r.z));
		}

		return r;
	}

	float Primitive::computeJointK() const 
	{
		Geometry::GeometryType type = geometry->getGeometryType();
		return Constants::getJointK(geometry->getGridSize(), type == Geometry::GeometryType::GEOMETRY_BALL);
	}

	void Primitive::setGuid(const Guid& value) 
	{
		RBXASSERT(world);
		guid.copyDataFrom(value);
		guidSetExternally = true;
	}

	void Primitive::setClump(Clump* set)
	{
		if(set != clump)
		{
			clump = set;
		}
	}

	const G3D::CoordinateFrame& Primitive::getCoordinateFrame() const
	{
		return body->getPV().position;
	}
	
	G3D::CoordinateFrame Primitive::getGridCorner() const
	{
		const G3D::CoordinateFrame& pos = body->getPV().position;
		G3D::Vector3 hVec = -(geometry->getGridSize() * 0.5f);

		return G3D::CoordinateFrame(pos.rotation, pos.pointToWorldSpace(hVec));
	}

	void Primitive::setGridCorner(const G3D::CoordinateFrame& gridCorner)
	{
		G3D::Vector3 kProd;
		G3D::Vector3 hVec = geometry->getGridSize() * 0.5;

		setCoordinateFrame(G3D::CoordinateFrame(gridCorner.rotation, gridCorner.pointToWorldSpace(hVec)));

	}

	void Primitive::setVelocity(const Velocity& vel)
	{
		body->setVelocity(vel);
	}

	float Primitive::getPlanarSize() const
	{
		return Math::planarSize(geometry->getGridSize());
	}

	Extents Primitive::getExtentsWorld() const 
	{

		G3D::Vector3 hVec = geometry->getGridSize() * 0.5f;
		Extents local(-hVec, hVec);

		return local.toWorldSpace(getCoordinateFrame());
	}

	bool Primitive::hitTest(const G3D::Ray& worldRay, G3D::Vector3& worldHitPoint, bool& inside) 
	{
		G3D::Ray localRay = body->getPV().position.toObjectSpace(worldRay);
		G3D::Vector3 localHitPoint;

		if(geometry->hitTest(localRay, localHitPoint, inside))
		{
			worldHitPoint = body->getPV().position.pointToWorldSpace(localHitPoint);
			return true;
		}
		else return false;
	}

	Face Primitive::getFaceInObject(NormalId objectFace)
	{
		G3D::Vector3 hVec = geometry->getGridSize() * 0.5;
		Extents extents(-hVec, hVec);

		return Face::fromExtentsSide(extents, objectFace);
	}

	Face Primitive::getFaceInWorld(NormalId objectFace)
	{
		return getFaceInObject(objectFace).toWorldSpace(body->getPV().position);
	}

	G3D::CoordinateFrame Primitive::getFaceCoordInObject(NormalId objectFace)
	{
		return G3D::CoordinateFrame(normalIdToMatrix3(objectFace), normalIdToVector3(objectFace) * geometry->getGridSize());
	}

	void Primitive::setSurfaceType(NormalId id, SurfaceType newSurfaceType)
	{
		if(surfaceType[id] != newSurfaceType)
			surfaceType[id] = newSurfaceType;
	}

	void Primitive::setSurfaceData(NormalId id, const SurfaceData& newSurfaceData)
	{
		if(!surfaceData[id] && newSurfaceData.isEmpty())
			return;

		if(surfaceData[id] && *surfaceData[id] == newSurfaceData)
			return;

		if(newSurfaceData.isEmpty())
		{
			RBXASSERT(surfaceData[id]);
			delete surfaceData[id];
			surfaceData[id] = NULL;
			return;
		}
		else if(!surfaceData[id])
		{
			surfaceData[id] = new SurfaceData;
		}

		*surfaceData[id] = newSurfaceData;
	}

	Edge* Primitive::getFirstEdge() const
	{
		return joints.first ? joints.first : contacts.first;
	}

	RigidJoint* Primitive::getFirstRigid()
	{
		return getFirstRigidAt(getFirstEdge());
	}

	Geometry* Primitive::newGeometry(Geometry::GeometryType geometryType)
	{
		switch(geometryType)
		{
			case Geometry::GeometryType::GEOMETRY_BLOCK: return new Block;
			case Geometry::GeometryType::GEOMETRY_BALL: return new Ball;
			default: return Geometry::nullGeometry();
		}
	}

	void newTouch(Primitive* touch, Primitive* touchOther); // TODO: Compiler complains about World class being undefined

	void Primitive::onNewTouch(Primitive* p0, Primitive* p1)
	{
		newTouch(p0, p1);
		newTouch(p1, p0);
	}
}
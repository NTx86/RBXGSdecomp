#pragma once
#include "v8world/IPipelined.h"
#include "v8world/Geometry.h"
#include "v8world/SurfaceData.h"
#include "util/Guid.h"
#include "util/Vector3int32.h"
#include "util/Extents.h"
#include "util/SurfaceType.h"
#include "util/ComputeProp.h"
#include "util/Face.h"
#include "util/Velocity.h"

namespace RBX
{
	class Edge;
	class Clump;
	class World;
	class SpatialNode;
	class Controller;
	class Assembly;
	class IMoving;
	class Anchor;
	class Joint;
	class RigidJoint;
	class Contact;
	class Primitive;

	class EdgeList
	{
	public:
		RBX::Edge* first;
		int num;
  
	public:
		EdgeList();
		~EdgeList();
	public:
		bool hasEdge();
  
	public:
		static void insertEdge(Primitive*, Edge*, EdgeList&);
		static void removeEdge(Primitive*, Edge*, EdgeList&);
	};

	class Primitive : public IPipelined
	{
		friend class SpatialHash;

	private:
		Guid guid;
		bool guidSetExternally;
		EdgeList contacts;
		EdgeList joints;
		int worldIndex;
		World* world;
		Clump* clump;
		int clumpDepth;
		int traverseId;
		SpatialNode* spatialNodes;
		Vector3int32 oldSpatialMin;
		Vector3int32 oldSpatialMax;
		Extents fuzzyExtents;
		int fuzzyExtentsStateId;
	protected:
		Geometry* geometry;
		Body* body;
		IMoving* myOwner;
		Anchor* anchorObject;
		bool dragging;
		bool anchored;
		bool canCollide;
		bool canSleep;
		float friction;
		float elasticity;
		SurfaceType surfaceType[6];
		SurfaceData* surfaceData[6];
		Controller* controller;
		ComputeProp<float, Primitive> JointK;
	public:
		static bool disableSleep;
	private:
		static bool ignoreBool;
  
	public:
		int& worldIndexFunc();
	private:
		void onChangedInKernel();
		G3D::Vector3 clipToSafeSize(const G3D::Vector3&);
		Extents computeFuzzyExtents() const;
	protected:
		float computeJointK() const;
	public:
		//Primitive(const Primitive&);
		Primitive(Geometry::GeometryType);
		virtual ~Primitive();
	public:
		const Guid& getGuid() const;
		void setGuid(const Guid& value);
		World* getWorld() const;
		void setWorld(World*);
		Clump* getClump() const
		{
			return clump;
		}
		void setClump(Clump* clump);
		int getClumpDepth() const
		{
			return clumpDepth;
		}
		void setClumpDepth(int value)
		{
			clumpDepth = value;
		}
		int getTraverseId() const;
		void setTraverseId(int);
		Assembly* getAssembly() const;
		const Geometry* getGeometry() const
		{
			return geometry;
		}
		Geometry* getGeometry()
		{
			return geometry;
		}
		void setPrimitiveType(Geometry::GeometryType geometryType);
		Geometry::GeometryType getPrimitiveType() const;
		const Body* getBody() const
		{
			return body;
		}
		Body* getBody()
		{
			return body;
		}
		void setOwner(IMoving*);
		IMoving* getOwner() const
		{
			return myOwner;
		}
		const G3D::CoordinateFrame& getCoordinateFrame() const;
		G3D::CoordinateFrame getGridCorner() const;
		void setCoordinateFrame(const CoordinateFrame& cFrame);
		void setGridCorner(const CoordinateFrame& gridCorner);
		void setPV(const PV&);
		void setVelocity(const Velocity& vel);
		void setMassInertia(float);
		void setDragging(bool dragging);
		bool getDragging() const
		{
			return dragging;
		}
		void setAnchor(bool anchor);
		bool getAnchor() const
		{
			return anchorObject != NULL;
		}
		Anchor* getAnchorObject()
		{
			return anchorObject;
		}
		void setCanCollide(bool canCollide);
		const bool getCanCollide() const
		{
			return canCollide;
		}
		void setCanSleep(bool canSleep);
		const bool getCanSleep() const
		{
			return canSleep;
		}
		void setFriction(float friction);
		float getFriction() const
		{
			return this->friction;
		}
		void setElasticity(float elasticity);
		float getElasticity() const
		{
			return this->elasticity;
		}
		void setGridSize(const Vector3& gridSize);
		const G3D::Vector3& getGridSize() const
		{
			return geometry->getGridSize();
		}
		virtual float getRadius() const;
		float getPlanarSize() const;
		Extents getExtentsLocal() const;
		Extents getExtentsWorld() const;
		const Extents& getFastFuzzyExtents() const;
		bool hitTest(const Ray& worldRay, Vector3& worldHitPoint, bool& inside);
		Face getFaceInObject(NormalId objectFace);
		Face getFaceInWorld(NormalId objectFace);
		G3D::CoordinateFrame getFaceCoordInObject(NormalId objectFace);
		void setSurfaceType(NormalId id, SurfaceType newSurfaceType);
		SurfaceType getSurfaceType(NormalId id) const
		{
			return surfaceType[id];
		}
		void setSurfaceData(NormalId id, const SurfaceData& newSurfaceData);
		const SurfaceData& getSurfaceData(NormalId id) const
		{
			// TODO: get this fully matching
			// check RotateJoint::getChannelValue
			const SurfaceData* data = surfaceData[id];
			return data ? *data : SurfaceData::empty();
		}
		void setController(Controller* controller);
		Controller* getController()
		{
			return controller;
		}
		float getJointK() const
		{
			return JointK;
		}
		RigidJoint* getFirstRigidAt(Edge* edge);
		int getNumJoints2() const
		{
			return joints.num;
		}
		int countNumJoints() const;
		int getNumContacts() const;
		int getNumEdges() const;
		bool hasEdge();
		Edge* getFirstEdge() const;
		Edge* getNextEdge(Edge* e) const;
		Joint* getFirstJoint() const;
		Joint* getNextJoint(Joint* prev) const;
		Contact* getFirstContact();
		Contact* getNextContact(Contact* prev);
		RigidJoint* getFirstRigid();
		RigidJoint* getNextRigid(RigidJoint* prev);
		//Primitive& operator=(const Primitive&);
  
	private:
		static const int fuzzyExtentsReset();
	protected:
		static Geometry* newGeometry(Geometry::GeometryType geometryType);
	public:
		static void onNewTouch(Primitive*, Primitive*);
		static float squaredDistance(const Primitive&, const Primitive&);
		static bool aaBoxCollide(const Primitive&, const Primitive&);
		static float defaultElasticity();
		static float defaultFriction();
		static void insertEdge(Edge*);
		static void removeEdge(Edge* e);
		static Joint* getJoint(Primitive* p0, Primitive* p1);
		static Contact* getContact(Primitive* p0, Primitive* p1);
		static Primitive* downstreamPrimitive(Joint*);
	};
}

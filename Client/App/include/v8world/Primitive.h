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
		void setGuid(const Guid&);
		World* getWorld() const;
		void setWorld(World*);
		Clump* getClump() const
		{
			return clump;
		}
		void setClump(Clump*);
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
		void setPrimitiveType(Geometry::GeometryType);
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
		void setCoordinateFrame(const G3D::CoordinateFrame&);
		void setGridCorner(const G3D::CoordinateFrame&);
		void setPV(const PV&);
		void setVelocity(const Velocity&);
		void setMassInertia(float);
		void setDragging(bool);
		bool getDragging() const
		{
			return dragging;
		}
		void setAnchor(bool);
		bool getAnchor() const;
		Anchor* getAnchorObject()
		{
			return anchorObject;
		}
		void setCanCollide(bool);
		const bool getCanCollide() const
		{
			return canCollide;
		}
		void setCanSleep(bool);
		const bool getCanSleep() const
		{
			return canSleep;
		}
		void setFriction(float);
		float getFriction() const;
		void setElasticity(float);
		float getElasticity() const;
		void setGridSize(const G3D::Vector3&);
		const G3D::Vector3& getGridSize() const
		{
			return geometry->getGridSize();
		}
		virtual float getRadius() const;
		float getPlanarSize() const;
		Extents getExtentsLocal() const;
		Extents getExtentsWorld() const;
		const Extents& getFastFuzzyExtents() const;
		bool hitTest(const G3D::Ray&, G3D::Vector3&, bool&);
		Face getFaceInObject(NormalId);
		Face getFaceInWorld(NormalId);
		G3D::CoordinateFrame getFaceCoordInObject(NormalId);
		void setSurfaceType(NormalId, SurfaceType);
		SurfaceType getSurfaceType(NormalId id) const
		{
			return surfaceType[id];
		}
		void setSurfaceData(NormalId, const SurfaceData&);
		const SurfaceData& getSurfaceData(NormalId id) const
		{
			// TODO: get this fully matching
			// check RotateJoint::getChannelValue
			const SurfaceData* data = surfaceData[id];
			return data ? *data : SurfaceData::empty();
		}
		void setController(Controller*);
		Controller* getController()
		{
			return controller;
		}
		float getJointK() const
		{
			return JointK;
		}
		RigidJoint* getFirstRigidAt(Edge*);
		int getNumJoints2() const;
		int countNumJoints() const;
		int getNumContacts() const;
		int getNumEdges() const;
		bool hasEdge();
		Edge* getFirstEdge() const;
		Edge* getNextEdge(Edge*) const;
		Joint* getFirstJoint() const;
		Joint* getNextJoint(Joint*) const;
		Contact* getFirstContact();
		Contact* getNextContact(Contact*);
		RigidJoint* getFirstRigid();
		RigidJoint* getNextRigid(RigidJoint*);
		//Primitive& operator=(const Primitive&);
  
	private:
		static const int fuzzyExtentsReset();
	protected:
		static Geometry* newGeometry(Geometry::GeometryType);
	public:
		static void onNewTouch(Primitive*, Primitive*);
		static float squaredDistance(const Primitive&, const Primitive&);
		static bool aaBoxCollide(const Primitive&, const Primitive&);
		static float defaultElasticity();
		static float defaultFriction();
		static void insertEdge(Edge*);
		static void removeEdge(Edge*);
		static Joint* getJoint(Primitive*, Primitive*);
		static Contact* getContact(Primitive*, Primitive*);
		static Primitive* downstreamPrimitive(Joint*);
	};
}

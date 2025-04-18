#pragma once
#include <set>
#include <boost/scoped_ptr.hpp>
#include <G3DAll.h>
#include "v8world/IWorldStage.h"
#include "v8world/Primitive.h"
#include "util/IndexArray.h"
#include "util/Events.h"
#include "util/Profiling.h"

namespace RBX
{
	class Assembly;
	class Joint;
	class Contact;
	class MotorJoint;
	class Kernel;
	class ContactManager;
	class SleepStage;
	class JointStage;
	class SimJobStage;
	class ClumpStage;
	class CollisionStage;

	// is this the right file for AutoJoin and AutoDestroy?
	struct AutoJoin
	{
	public:
		Joint* joint;
		AutoJoin(Joint*);
	};

	struct AutoDestroy
	{
	public:
		Joint* joint;
		AutoDestroy(Joint*);
	};

	class World : public Notifier<World, AutoJoin>, public Notifier<World, AutoDestroy>
	{
	private:
		ContactManager* contactManager;
		JointStage* jointStage;
		G3D::Array<Primitive*> touch;
		G3D::Array<Primitive*> touchOther;
		bool canThrottle;
		bool inStepCode;
		bool inJointNotification;
		int worldStepId;
		RBX::IndexArray<Primitive, &Primitive::worldIndexFunc> primitives;
		std::set<Joint*> breakableJoints;
		int numJoints;
		int numContacts;
		int numLinkCalls;
		G3D::Array<Primitive*> tempPrimitives;
		boost::scoped_ptr<Profiling::CodeProfiler> profilingWorldStep;
		boost::scoped_ptr<Profiling::CodeProfiler> profilingUiStep;
		boost::scoped_ptr<Profiling::CodeProfiler> profilingBroadphase;
	public:
		static bool disableEnvironmentalThrottle;
  
	public:
		void createJoints(Primitive*);
	private:
		void createJoints(Primitive*, std::set<Primitive*>*);
	public:
		void destroyJoints(Primitive*);
	private:
		void destroyJoints(Primitive*, std::set<Primitive*>*);
		void destroyJoint(Joint*);
		void removeFromBreakable(Joint*);
		void doBreakJoints();
	public:
		//World(const World&);
		World();
		virtual ~World();
	public:
		void assertNotInStep();
		void assertInStep();
		void addedBodyForce();
		void setCanThrottle(bool);
		ContactManager& getContactManager();
		ClumpStage* getClumpStage();
		const CollisionStage* getCollisionStage() const;
		CollisionStage* getCollisionStage();
		const SleepStage* getSleepStage() const;
		SleepStage* getSleepStage();
		SimJobStage& getSimJobStage();
		const Kernel& getKernel() const;
		Kernel& getKernel();
		const G3D::Array<Primitive*>& getTouch() const;
		const G3D::Array<Primitive*>& getTouchOther() const;
		void computeFallen(G3D::Array<Primitive*>&) const;
		const G3D::Array<Primitive*>& getPrimitives() const;
		float step(float);
		void update();
		void reset();
		int getWorldStepId();
		void insertPrimitive(Primitive*);
		void removePrimitive(Primitive*);
		void ticklePrimitive(Primitive*);
		void joinAll();
		void createJointsToWorld(const G3D::Array<Primitive*>&);
		void destroyJointsToWorld(const G3D::Array<Primitive*>&);
		void insertJoint(Joint*);
		void removeJoint(Joint*);
		int getMetric(IWorldStage::MetricType) const;
		int getNumBodies() const;
		int getNumPoints() const;
		int getNumConstraints() const;
		int getNumHashNodes() const;
		int getMaxBucketSize() const;
		int getNumLinkCalls() const;
		int getNumContacts() const;
		int getNumJoints() const;
		int getNumPrimitives() const;
		const Profiling::CodeProfiler& getProfileWorldStep() const;
		Profiling::CodeProfiler& getProfileWorldStep();
		const Profiling::CodeProfiler& getProfileBroadphase() const;
		const Profiling::CodeProfiler& getProfileUiStep() const;
		void onPrimitiveAddedAnchor(Primitive*);
		void onPrimitiveRemovingAnchor(Primitive*);
		void onPrimitiveExtentsChanged(Primitive*);
		void onAssemblyExtentsChanged(Assembly*);
		void onPrimitiveContactParametersChanged(Primitive*);
		void onPrimitiveCanCollideChanged(Primitive*);
		void onPrimitiveCanSleepChanged(Primitive*);
		void onPrimitiveGeometryTypeChanged(Primitive*);
		void onPrimitiveTouched(Primitive*, Primitive*);
		void onMotorAngleChanged(MotorJoint*);
		void onJointPrimitiveNulling(Joint*, Primitive*);
		void onJointPrimitiveSet(Joint*, Primitive*);
		void insertContact(Contact*);
		void destroyContact(Contact*);
		//World& operator=(const World&);
	};
}

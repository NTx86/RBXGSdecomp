#pragma once
#include <boost/scoped_ptr.hpp>
#include "v8world/IWorldStage.h"
#include "v8world/Contact.h"
#include "util/IndexArray.h"
#include "util/Profiling.h"

namespace RBX
{
	class Edge;
	class Joint;
	class World;
	class Assembly;
	
	class CollisionStage : public IWorldStage
	{
	private:
		int numContactsInStage;
		RBX::IndexArray<Contact, &Contact::steppingIndexFunc> stepping;
		boost::scoped_ptr<Profiling::CodeProfiler> profilingCollision;
	  
	private:
		bool getOkToSim(Contact*, bool&);
		bool confirmOneClumpHere(Edge*);
		RBX::Contact* getContactBetweenPrimitives(Joint*);
		void removeContactFromDownstreamStage(Contact*);
		void removeContact(Contact*);
		void updateContact(Contact*);
		void onJointAdded(Joint*);
		void onJointRemoved(Joint*);
	public:
		//CollisionStage(const CollisionStage&);
		CollisionStage(IStage*, World*);
		virtual ~CollisionStage();
	public:
		virtual IStage::StageType getStageType();
		virtual void stepWorld(int, int, bool);
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		virtual int getMetric(IWorldStage::MetricType);
		void onAssemblyAdded(Assembly*);
		void onAssemblyRemoving(Assembly*);
		void onPrimitiveCanCollideChanged(Primitive*);
		void onSleepChanged(const Assembly*);
		void onLosingContact(const G3D::Array<Contact*>&);
		//CollisionStage& operator=(const CollisionStage&);
	};
}

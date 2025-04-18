#pragma once
#include <set>
#include <boost/scoped_ptr.hpp>
#include <G3DAll.h>
#include "v8world/IWorldStage.h"
#include "util/Profiling.h"

namespace RBX
{
	namespace Sim
	{
		enum AssemblyState;
	}

	class Assembly;
	class CollisionStage;
	class World;
	class Edge;
	class Contact;

	class SleepStage : public IWorldStage
	{
	private:
		std::set<Assembly*> awake;
		std::set<Assembly*> sleepingChecking;
		std::set<Assembly*> sleepingDeeply;
	public:
		boost::scoped_ptr<Profiling::CodeProfiler> profilingSleep;
  
	private:
		std::set<Assembly*>& statusToArray(Sim::AssemblyState);
		void remove(Assembly*);
		void insert(Assembly*, Sim::AssemblyState);
		void changeSleepStatus(Assembly*, Sim::AssemblyState);
		bool shouldSleep(Assembly*);
		Sim::AssemblyState shouldWakeOrSleepDeeply(Assembly*);
		void wakeAssembly(Assembly*);
		void goToSleep(Assembly*);
		void wakeAssemblyAndNeighbors(Assembly*, bool);
		void wakeAssemblyAndNeighbors(Assembly*, int);
		void checkAwakeAssemblies(bool);
		void checkSleepingAssemblies();
		void validateEdge(Edge*);
		bool debugValidate();
		CollisionStage* getCollisionStage();
	public:
		//SleepStage(const SleepStage&);
		SleepStage(IStage*, World*);
		virtual ~SleepStage();
	public:
		virtual IStage::StageType getStageType();
		virtual void stepWorld(int, int, bool);
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		void onAssemblyAdded(Assembly*);
		void onAssemblyRemoving(Assembly*);
		void onWakeUpRequest(Assembly*);
		int numTouchingContacts();
		const std::set<Assembly*>& getAwakeAssemblies() const;
		void onLosingContact(const Array<Contact*>&);
		//SleepStage& operator=(const SleepStage&);
  
	private:
		static int stepsToSleep();
	};
}
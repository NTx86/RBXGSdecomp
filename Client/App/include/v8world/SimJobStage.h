#pragma once
#include <list>
#include "v8world/IWorldStage.h"

namespace RBX
{
	class Mechanism;
	class Assembly;
	class World;
	class Edge;

	class SimJobStage : public IWorldStage
	{
	private:
		std::list<Mechanism*> mechanisms;
	  
	private:
		void combineMechanisms(Edge*);
		void insertMechanism(Mechanism*);
		void destroyMechanism(Mechanism*);
		void splitMechanisms(Assembly*, Assembly*);
		bool validateEdge(Edge*);
	public:
		Mechanism* nextMechanism(std::list<Mechanism*>&, const Mechanism*);
	public:
		//SimJobStage(const SimJobStage&);
		SimJobStage(IStage*, World*);
		virtual ~SimJobStage();
	public:
		virtual IStage::StageType getStageType();
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		void onAssemblyAdded(Assembly*);
		void onAssemblyRemoving(Assembly*);
		void notifyMovingPrimitives();
		//SimJobStage& operator=(const SimJobStage&);
	};
}

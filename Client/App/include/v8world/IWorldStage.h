#pragma once
#include "v8kernel/IStage.h"

namespace RBX
{
	class World;
	class Edge;

	class IWorldStage : public IStage
	{
	public:
		enum MetricType
		{
			NUM_CONTACTSTAGE_CONTACTS,
			NUM_STEPPING_CONTACTS,
			NUM_TOUCHING_CONTACTS,
			MAX_TREE_DEPTH
		};

	private:
		RBX::World* world;
  
	public:
		//IWorldStage(const IWorldStage&);
		IWorldStage(IStage*, IStage*, World*);
	public:
		IWorldStage* getUpstreamWS();
		IWorldStage* getDownstreamWS();
		World* getWorld();
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		virtual int getMetric(MetricType);
		virtual ~IWorldStage();
		//IWorldStage& operator=(const IWorldStage&);
	};
}

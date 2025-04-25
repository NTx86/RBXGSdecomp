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
		World* world;
  
	public:
		//IWorldStage(const IWorldStage&);
		IWorldStage(IStage* upstream, IStage* downstream, World* world)
			: IStage(upstream, downstream),
			  world(world)
		{
		}
	public:
		IWorldStage* getUpstreamWS();
		IWorldStage* getDownstreamWS()
		{
			return rbx_static_cast<IWorldStage*>(getDownstream());
		}
		World* getWorld() { return world; }
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		virtual int getMetric(MetricType);
	public:
		virtual ~IWorldStage();
	public:
		//IWorldStage& operator=(const IWorldStage&);
	};
}

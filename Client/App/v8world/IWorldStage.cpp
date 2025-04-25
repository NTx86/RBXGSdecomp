#include "v8world/IWorldStage.h"
#include "v8world/Edge.h"

namespace RBX
{
	void IWorldStage::onEdgeAdded(Edge* e)
	{
		RBXAssert(getDownstreamWS());
		e->putInPipeline(this);
		getDownstreamWS()->onEdgeAdded(e);
	}

	void IWorldStage::onEdgeRemoving(Edge* e)
	{
		RBXAssert(getDownstreamWS());
		getDownstreamWS()->onEdgeRemoving(e);
		e->removeFromStage(this);
	}

	int IWorldStage::getMetric(MetricType metricType)
	{
		RBXAssert(getDownstreamWS());
		return getDownstreamWS()->getMetric(metricType);
	}
}

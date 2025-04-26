#pragma once
#include <g3d/array.h>
#include "v8world/IWorldStage.h"

namespace RBX
{
	class Contact;
	class Assembly;
	class SleepStage;

	class SeparateStage : public IWorldStage
	{
	private:
		G3D::Array<Contact*> separating;
		std::set<Contact*> inContact;
	  
	private:
		SleepStage* getSleepStage();
	public:
		SeparateStage(const SeparateStage&);
		SeparateStage(IStage*, World*);
		virtual ~SeparateStage();
	public:
		virtual StageType getStageType();
		virtual void stepWorld(int, int, bool);
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		virtual int getMetric(MetricType);
		void onAssemblyAdded(Assembly*);
		void onAssemblyRemoving(Assembly*);
		//SeparateStage& operator=(const SeparateStage&);
	};
}

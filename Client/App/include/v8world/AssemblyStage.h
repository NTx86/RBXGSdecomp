#pragma once
#include <set>
#include "v8world/IWorldStage.h"

namespace RBX
{
	class Assembly;
	class Joint;
	class World;
	class Edge;

	class AssemblyStage : public IWorldStage
	{
	private:
		std::set<Assembly*> assemblies;
		std::set<Joint*> joints;
  
	private:
		void onJointAdded(Joint*);
		void onJointRemoving(Joint*);
	public:
		//AssemblyStage(const AssemblyStage&);
		AssemblyStage(IStage*, World*);
		virtual ~AssemblyStage();
	public:
		virtual StageType getStageType()
		{
			return ASSEMBLY_STAGE;
		}
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		void stepUi(int);
		void onAssemblyAdded(Assembly*);
		void onAssemblyRemoving(Assembly*);
		void wakeAssembly(Assembly*);
		//AssemblyStage& operator=(const AssemblyStage&);
	};
}

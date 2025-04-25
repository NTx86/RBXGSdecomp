#pragma once
#include "v8kernel/Kernel.h"
#include "v8kernel/IStage.h"
#include "v8world/IWorldStage.h"

namespace RBX
{
	class World;

	class IPipelined
	{
	private:
		RBX::IStage* currentStage;
  
	public:
		void removeFromStage(IStage* stage);
	private:
		void removeFromStage(IStage::StageType);
		IStage* getStage(IStage::StageType stageType) const;
	public:
		//IPipelined(const IPipelined&);
		IPipelined() : currentStage(NULL) {}
		virtual ~IPipelined()
		{
			RBXAssert(currentStage == NULL);
		}
		void putInPipeline(IStage* stage);
		void removeFromPipeline(IStage* stage);
		void putInStage(IStage* stage);
		bool inPipeline() const
		{
			return currentStage != NULL;
		}
		bool inStage(IStage* iStage) const
		{
			RBXAssert(iStage);
			RBXAssert(currentStage);

			return currentStage == iStage;
		}
		bool inStage(IStage::StageType stageType) const
		{
			RBXAssert(currentStage);

			return currentStage->getStageType() == stageType;
		}
		bool inOrDownstreamOfStage(IStage* iStage) const
		{
			RBXAssert(iStage);
			RBXAssert(currentStage);

			return (int)currentStage->getStageType() >= (int)iStage->getStageType();
		}
		bool downstreamOfStage(IStage* iStage) const
		{
			RBXAssert(iStage);
			RBXAssert(currentStage);

			return (int)currentStage->getStageType() > (int)iStage->getStageType();
		}
		bool inKernel() const
		{
			return this->inStage(IStage::KERNEL_STAGE);
		}
		Kernel* getKernel() const;
		virtual void putInKernel(Kernel* kernel);
		virtual void removeFromKernel();
		World* getWorld()
		{
			if (!currentStage)
				return NULL;

			IStage* upstream;
			if (currentStage->getStageType() != IStage::KERNEL_STAGE)
				upstream = currentStage;
			else
				upstream = currentStage->getUpstream();

			return rbx_static_cast<IWorldStage*>(upstream)->getWorld();
		}
		//IPipelined& operator=(const IPipelined&);
	};
}

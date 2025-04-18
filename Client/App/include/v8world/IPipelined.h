#pragma once
#include "v8kernel/Kernel.h"
#include "v8kernel/IStage.h"

namespace RBX
{
	class World;

	class IPipelined
	{
	private:
		IStage* currentStage;
  
	public:
		void removeFromStage(IStage*);
	private:
		void removeFromStage(IStage::StageType);
		IStage* getStage(IStage::StageType) const;
	public:
		//IPipelined(const RBX::IPipelined&);
		IPipelined();
		virtual ~IPipelined();
		void putInPipeline(IStage*);
		void removeFromPipeline(IStage*);
		void putInStage(IStage*);
		bool inPipeline() const;
		bool inStage(IStage*) const;
		bool inStage(IStage::StageType) const;
		bool inOrDownstreamOfStage(IStage*) const;
		bool downstreamOfStage(IStage*) const;
		bool inKernel() const;
		Kernel* getKernel() const;
		virtual void putInKernel(Kernel*);
		virtual void removeFromKernel();
		World* getWorld();
		//IPipelined& operator=(const IPipelined&);
	};
}

#include "v8world/IPipelined.h"
#include "v8kernel/Kernel.h"

namespace RBX
{
	IStage* IPipelined::getStage(IStage::StageType stageType) const
	{
		RBXAssert(currentStage);

		IStage* stage = currentStage;
		while (stage->getStageType() != stageType)
		{
			if ((int)stage->getStageType() > (int)stageType)
				stage = stage->getUpstream();
			else
				stage = stage->getDownstream();
		}
		return stage;
	}

	void IPipelined::putInPipeline(IStage* stage)
	{
		RBXAssert(stage);
		RBXAssert(!currentStage);

		currentStage = stage;
	}

	void IPipelined::removeFromPipeline(IStage* stage)
	{
		RBXAssert(stage);
		RBXAssert(currentStage);
		RBXAssert(currentStage == stage);

		currentStage = NULL;
	}

	void IPipelined::putInStage(IStage* stage)
	{
		RBXAssert(stage);
		RBXAssert(currentStage);
		RBXAssert(stage->getUpstream() == currentStage);
		RBXAssert(currentStage->getDownstream() == stage);

		currentStage = stage;
	}

	void IPipelined::removeFromStage(IStage* stage)
	{
		RBXAssert(currentStage);
		RBXAssert(stage);
		RBXAssert(stage == currentStage);
		RBXAssert(stage->getUpstream());

		currentStage = currentStage->getUpstream();
	}

	Kernel* IPipelined::getKernel() const
	{
		RBXAssert(this->inKernel());

		IStage* stage = this->getStage(IStage::KERNEL_STAGE);
		return rbx_static_cast<Kernel*>(stage);
	}

	void IPipelined::putInKernel(Kernel* kernel)
	{
		this->putInStage((IStage*)kernel);
	}

	void IPipelined::removeFromKernel()
	{
		RBXAssert(currentStage);
		RBXAssert(currentStage->getStageType() == IStage::KERNEL_STAGE);

		this->removeFromStage(currentStage);
	}
}

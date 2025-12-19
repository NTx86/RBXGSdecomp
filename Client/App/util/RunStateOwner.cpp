#include "util/RunStateOwner.h"

namespace RBX
{
	void RunService::endRunThread(bool join)
	{
		stopRequested = true;
		stateChangedCondition.notify_all();

		if (join)
			runThread->join();
	}

	void RunService::onAncestorChanged(const AncestorChanged& event)
	{
		if (!getParent())
		{
			stopRequested = true;
			stateChangedCondition.notify_all();
		}
	}

	void RunService::setRunState(RunState newState)
	{
		RunTransition transition(runState, newState);
		runState = newState;

		Notifier<RunService, RunState>::raise(newState);
		Notifier<RunService, RunTransition>::raise(transition);
	}

	void RunService::pause()
	{
		if (runState == RS_RUNNING)
			setRunState(RS_PAUSED);
	}

	void RunService::reset()
	{
		if (runState != RS_NORMAL)
			setRunState(RS_NORMAL);
	}

	void RunService::run()
	{
		if (!runDisabled && (runState == RS_NORMAL || runState == RS_PAUSED) && runState != RS_RUNNING)
			setRunState(RS_RUNNING);
	}

	void RunService::raiseStepped(float time, float step)
	{
		Notifier<RunService, Stepped>::raise(Stepped(time, step));
		event_Stepped.fire(this, time, step);
	}

	void RunService::invalidateRunViews()
	{
		boost::mutex::scoped_lock lock(viewMutex);

		if (invalidRunViewCount == 0)
		{
			invalidRunViewCount = (int)views.size();
			if (invalidRunViewCount > 0)
			{
				for (std::map<IRunView*, bool>::iterator iter = views.begin(); iter != views.end(); iter++)
				{
					iter->first->InvalidateRunView();
					iter->second = false;
				}
				runViewsDoneCondition.notify_all();
			}
		}
	}
}

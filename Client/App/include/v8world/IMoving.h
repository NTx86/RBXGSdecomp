#pragma once
#include <set>

namespace RBX
{
	class IMovingManager;
	class IMoving
	{
	private:
		IMovingManager* iMovingManager;
		int stepsToSleep;
  
	private:
		void makeMoving();
	protected:
		virtual void onCanAggregateChanged(bool);
		void setMovingManager(IMovingManager*);
		bool checkSleep();
	public:
		//IMoving(const IMoving&);
		IMoving();
		~IMoving();
	public:
		void notifyMoved();
		virtual bool reportTouches() const;
		bool getCanAggregate() const;
		//IMoving& operator=(const IMoving&);
	};

	class IMovingManager
	{
	private:
		std::set<IMoving*> moving;
		std::set<IMoving*>::iterator current;
	protected:
		void remove(IMoving*);
		void moved(IMoving*);
	public:
		//IMovingManager(const IMovingManager&);
		IMovingManager();
		virtual ~IMovingManager();
	public:
		void onMovingHeartbeat();
		//IMovingManager& operator=(const IMovingManager&);
	};
}

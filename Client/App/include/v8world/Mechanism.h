#pragma once
#include <boost/noncopyable.hpp>
#include <set>
#include <list>
#include <vector>

namespace RBX
{
	class Primitive;
	class Assembly;
	class MechanismTracker;

	class Mechanism : public boost::noncopyable
	{
	private:
		std::set<Assembly*> assemblies;
		std::vector<MechanismTracker*> trackers;
	public:
		std::list<Mechanism*>::iterator myIt;
  
	public:
		~Mechanism();
	public:
		const std::set<Assembly*>& getAssemblies() const;
		std::set<Assembly*>& getAssemblies();
		void notifyMovingPrimitives();
		void insertAssembly(Assembly*);
		void removeAssembly(Assembly*);
		void absorb(Mechanism*);
	public:
		//Mechanism(const Mechanism&);
		Mechanism();
	public:
		//Mechanism& operator=(const Mechanism&);
  
	public:
		static Mechanism* getMechanismFromPrimitive(const Primitive*);
	};

	class MechanismTracker
	{
	private:
		Mechanism* mechanism;
  
	private:
		bool containedBy(Mechanism*);
		void stopTracking();
	public:
		MechanismTracker();
		~MechanismTracker();
		bool tracking();
		void setMechanism(Mechanism*);
		Mechanism* getMechanism();
  
	public:
		static void transferTrackers(Mechanism*, Mechanism*);
	};
}

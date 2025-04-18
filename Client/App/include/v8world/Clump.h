#pragma once
#include <boost/noncopyable.hpp>
#include <set>
#include "util/ComputeProp.h"
#include "util/RunningAverage.h"

namespace RBX
{
	class Assembly;
	class Primitive;
	class Anchor;
	class RigidJoint;
	class Edge;
	class Kernel;
	class Body;

	namespace Sim
	{
		enum AssemblyState;
	}

	class SpanLink
	{
	public:
		Primitive* parent;
		RigidJoint* joint;
  
	public:
		SpanLink(Primitive*, RigidJoint*);
		SpanLink();
	public:
		Primitive* child() const;
  
	public:
		static Primitive* isSpanningJoint(RigidJoint*);
		static bool isDownstreamSpanningJoint(Primitive*, RigidJoint*);
		static bool isUpstreamSpanningJoint(Primitive*, RigidJoint*);
		static bool spansToChild(RigidJoint*, Primitive*);
		static SpanLink bestSpanLink(const SpanLink&, const SpanLink&);
	};

	class Clump : public boost::noncopyable
	{
	private:
		Sim::AssemblyState sleepStatus;
		RunningAverageState runningAverageState;
		Assembly* assembly;
		Primitive* rootPrimitive;
		Anchor* anchor;
		bool canSleep;
		std::set<Primitive*> primitives;
		std::set<RigidJoint*> inconsistentJoints;
		ComputeProp<float, Clump> MaxRadius;
	private:
		static Primitive* tempChild;
  
	private:
		float computeMaxRadius() const;
		Body* getRootBody() const;
		bool computeCanSleep();
		bool containsPrimitive(Primitive*) const;
		void setParent(Primitive*, Primitive*);
		void setSpanParent(Primitive*, RigidJoint*);
		void renumberSpanTree(Primitive*);
		SpanLink findBestOutsideLink(Primitive*, Primitive*);
		bool inSpanTree(Primitive*, Primitive*);
		bool okSpanTree(Primitive*, RigidJoint*);
	public:
		//Clump(const Clump&);
		Clump(Primitive*);
		~Clump();
	public:
		const Primitive* getRootPrimitive() const;
		Primitive* getRootPrimitive();
		const Assembly* getRootAssembly() const;
		Assembly* getRootAssembly();
		Assembly* getAssembly() const;
		void setAssembly(Assembly*);
		void addPrimitive(Primitive*, Primitive*, RigidJoint*);
		void removePrimitive(Primitive*);
		void removeAllPrimitives();
		void getRigidJoints(std::set<RigidJoint*>&, std::set<RigidJoint*>&);
		void addAnchor(Anchor*);
		Anchor* removeAnchor();
		bool getAnchored() const;
		bool getCanSleep() const;
		Sim::AssemblyState getSleepStatus();
		bool moving();
		void putInKernel(Kernel*);
		void removeFromKernel(Kernel*);
		bool calcShouldSleep();
		bool okNeighborSleep();
		bool forceNeighborAwake();
		std::set<RigidJoint*>& getInconsistents();
		void addInconsistent(RigidJoint*);
		void removeInconsistent(RigidJoint*);
		bool containsInconsistent(RigidJoint*);
		bool spanningTreeAdjust(RigidJoint*);
		void setSleepStatus(Sim::AssemblyState);
		void onPrimitiveCanSleepChanged(Primitive*);
		Clump* otherClump(Edge*) const;
		int size();
		std::set<Primitive*>::const_iterator clumpPrimBegin() const;
		std::set<Primitive*>::const_iterator clumpPrimEnd() const;
		//Clump& operator=(const Clump&);
  
	private:
		static float sleepTolerance();
	};
}

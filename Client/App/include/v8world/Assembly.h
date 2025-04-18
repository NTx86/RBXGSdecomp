#pragma once
#include <set>
#include <boost/noncopyable.hpp>
#include "v8world/IPipelined.h"

namespace RBX
{
	class Primitive;
	class Clump;
	class Edge;
	class MotorJoint;
	class Mechanism;
	class Kernel;

	namespace Sim
	{
		enum AssemblyState
		{
			ANCHORED,
			RECURSIVE_WAKE_PENDING,
			WAKE_PENDING,
			AWAKE,
			SLEEPING_CHECKING,
			SLEEPING_DEEPLY
		};
	}

	class Assembly : public IPipelined, public boost::noncopyable
	{
	public:
		class PrimIterator
		{
		private:
			const Assembly* assembly;
			std::set<Clump*>::const_iterator clumpIterator;
			std::set<Primitive*>::const_iterator primIterator;
  
		private:
			PrimIterator(const Assembly*, std::set<Clump*>::const_iterator, std::set<Primitive*>::const_iterator);
			PrimIterator& operator=(const PrimIterator&);
			Primitive* operator*() const;
			bool operator==(const PrimIterator&) const;
			bool operator!=(const PrimIterator&) const;
			PrimIterator& operator++();
  
		public:
			static PrimIterator begin(const RBX::Assembly*);
			static PrimIterator end(const RBX::Assembly*);
		};

	private:
		Clump* rootClump;
		std::set<Clump*> clumps;
		std::set<Edge*> internalEdges;
		std::set<Edge*> externalEdges;
		std::vector<MotorJoint*> motors;
		std::set<MotorJoint*> inconsistentMotors;
		Mechanism* mechanism;
		int sleepCount;
  
	private:
		void insertClump(Clump*);
		void updateMotorJoint(MotorJoint*);
	public:
		//Assembly(const Assembly&);
		Assembly(Clump*);
		virtual ~Assembly();
	public:
		void addClump(Clump*, MotorJoint*);
		void addClumpNew(Clump*, MotorJoint*);
		void removeClump(Clump*);
		void removeMotor(MotorJoint*);
		void removeAllMotors();
		Primitive* getMainPrimitive() const;
		Primitive* getAssemblyPrimitive() const;
		const Primitive* getAssemblyPrimitiveConst() const;
		Clump* getRootClump();
		const std::set<Clump*>& getClumps() const;
		std::set<Clump*>& getClumps();
		bool getCanSleep();
		bool getAnchored() const;
		Sim::AssemblyState getSleepStatus();
		bool moving();
		int getDof() const;
		int getSleepCount() const;
		void setSleepCount(int);
		void incrementSleepCount();
		virtual void putInKernel(Kernel*);
		virtual void removeFromKernel();
		void setMechanism(Mechanism*);
		Mechanism* getMechanism();
		void stepUi(int);
		bool calcShouldSleep();
		bool okNeighborSleep();
		bool forceNeighborAwake();
		void setSleepStatus(Sim::AssemblyState);
		void onPrimitiveCanSleepChanged(Primitive*);
		std::set<Edge*>& getExternalEdges();
		PrimIterator assemblyPrimBegin() const;
		PrimIterator assemblyPrimEnd() const;
		std::set<Edge*>::const_iterator externalEdgeBegin() const;
		std::set<Edge*>::const_iterator externalEdgeEnd() const;
		const std::set<Edge*>& getInternalEdges() const;
		std::set<Edge*>& getInternalEdges();
		std::vector<MotorJoint*>& getMotors();
		std::set<MotorJoint*>& getInconsistentMotors();
		void addExternalEdge(Edge*);
		void removeInternalEdge(Edge*);
		bool containsInternalEdge(Edge*);
		void addInternalEdge(Edge*);
		void removeExternalEdge(Edge*);
		bool containsExternalEdge(Edge*);
		void addInconsistentMotor(MotorJoint*);
		void removeInconsistentMotor(MotorJoint*);
		void notifyMoved();
		Assembly* otherAssembly(Edge*) const;
		unsigned int numMotors() const;
		MotorJoint* getMotor(unsigned int);
		const MotorJoint* getMotorConst(unsigned int) const;
		//Assembly& operator=(const Assembly&);
	};
}

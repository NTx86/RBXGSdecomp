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
			PrimIterator(
				const Assembly* assembly,
				std::set<Clump*>::const_iterator clumpIterator,
				std::set<Primitive*>::const_iterator primIterator)
				: assembly(assembly),
				  clumpIterator(clumpIterator),
				  primIterator(primIterator)
			{
			}
		public:
			PrimIterator& operator=(const PrimIterator& other)
			{
				assembly = other.assembly;
				clumpIterator = other.clumpIterator;
				primIterator = other.primIterator;
				return *this;
			}
			Primitive* operator*() const;
			bool operator==(const PrimIterator&) const;
			__forceinline bool operator!=(const PrimIterator& other) const
			{
				RBXAssert(clumpIterator == other.clumpIterator);
				return clumpIterator == other.clumpIterator && primIterator == other.primIterator && assembly == other.assembly;
			}
			PrimIterator& operator++();
  
		public:
			static PrimIterator begin(const Assembly* assembly);
			static PrimIterator end(const Assembly* assembly);
		};

	private:
		Clump* rootClump;
		std::set<Clump*> clumps;
		std::set<Edge*> internalEdges;
		std::set<Edge*> externalEdges;
		mutable std::vector<MotorJoint*> motors;
		std::set<MotorJoint*> inconsistentMotors;
		Mechanism* mechanism;
		int sleepCount;
  
	private:
		void insertClump(Clump* c);
		void updateMotorJoint(MotorJoint*);
	public:
		//Assembly(const Assembly&);
		Assembly(Clump* root);
		virtual ~Assembly();
	public:
		void addClump(Clump* c, MotorJoint* m);
		void addClumpNew(Clump*, MotorJoint*);
		void removeClump(Clump* c);
		void removeMotor(MotorJoint* m);
		void removeAllMotors();
		Primitive* getMainPrimitive() const;
		Primitive* getAssemblyPrimitive() const;
		const Primitive* getAssemblyPrimitiveConst() const;
		Clump* getRootClump();
		const std::set<Clump*>& getClumps() const
		{
			return clumps;
		}
		std::set<Clump*>& getClumps()
		{
			return clumps;
		}
		bool getCanSleep();
		bool getAnchored() const;
		Sim::AssemblyState getSleepStatus();
		bool moving();
		int getDof() const;
		int getSleepCount() const;
		void setSleepCount(int count);
		void incrementSleepCount();
		virtual void putInKernel(Kernel* _kernel);
		virtual void removeFromKernel();
		void setMechanism(Mechanism*);
		Mechanism* getMechanism();
		void stepUi(int uiStepId);
		bool calcShouldSleep();
		bool okNeighborSleep();
		bool forceNeighborAwake();
		void setSleepStatus(Sim::AssemblyState _set);
		void onPrimitiveCanSleepChanged(Primitive* p);
		std::set<Edge*>& getExternalEdges();
		PrimIterator assemblyPrimBegin() const;
		PrimIterator assemblyPrimEnd() const;
		std::set<Edge*>::const_iterator externalEdgeBegin() const;
		std::set<Edge*>::const_iterator externalEdgeEnd() const;
		const std::set<Edge*>& getInternalEdges() const;
		std::set<Edge*>& getInternalEdges();
		std::vector<MotorJoint*>& getMotors();
		std::set<MotorJoint*>& getInconsistentMotors();
		void addExternalEdge(Edge* e);
		void removeInternalEdge(Edge* e);
		bool containsInternalEdge(Edge* e);
		void addInternalEdge(Edge* e);
		void removeExternalEdge(Edge* e);
		bool containsExternalEdge(Edge* e);
		void addInconsistentMotor(MotorJoint* m);
		void removeInconsistentMotor(MotorJoint* m);
		void notifyMoved();
		Assembly* otherAssembly(Edge* edge) const;
		size_t numMotors() const;
		MotorJoint* getMotor(unsigned int motorId);
		const MotorJoint* getMotorConst(unsigned int motorId) const;
		//Assembly& operator=(const Assembly&);
	};
}

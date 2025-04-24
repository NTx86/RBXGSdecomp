#pragma once
#include <map>
#include <set>
#include <vector>
#include "v8world/IWorldStage.h"

namespace RBX
{
	class Anchor;
	class Primitive;
	class RigidJoint;
	class MotorJoint;
	class Clump;
	class Assembly;

	// MYSTERY CLASS: no size, only referenced here
	class Influence
	{
	};

	class PrimitiveSort
	{
	public:
		bool anchored;
		int surfaceAreaJoints;
	  
	public:
		//PrimitiveSort(const PrimitiveSort&);
		PrimitiveSort(const Primitive*);
		PrimitiveSort();
	public:
		bool operator==(const PrimitiveSort&) const;
		bool operator!=(const PrimitiveSort&) const;
		bool operator<(const PrimitiveSort& other) const
		{
			return anchored != other.anchored && surfaceAreaJoints < other.surfaceAreaJoints;
		}
		bool operator>(const PrimitiveSort& other) const
		{
			return anchored != other.anchored && surfaceAreaJoints > other.surfaceAreaJoints;
		}
	  
	private:
		static bool isEqual(const PrimitiveSort&, const PrimitiveSort&);
		static bool isLess(const PrimitiveSort&, const PrimitiveSort&);
	};

	class PrimitiveEntry
	{
	public:
		Primitive* primitive;
		PrimitiveSort power;

	public:
		//PrimitiveEntry(const PrimitiveEntry&);
		PrimitiveEntry(Primitive*, PrimitiveSort);
	};

	class AnchorEntry
	{
	public:
		Anchor* anchor;
		int size;

	public:
		AnchorEntry(Anchor* anchor, int size);
	};

	class RigidEntry
	{
	public:
		RigidJoint* rigidJoint;
		PrimitiveSort power;

	public:
		//RigidEntry(const RigidEntry&);
		RigidEntry(RigidJoint*, PrimitiveSort);
	};

	class PrimitiveSortCriterion
	{
	public:
		bool operator()(const PrimitiveEntry&, const PrimitiveEntry&) const;
	};

	class AnchorSortCriterion
	{
	public:
		bool operator()(const AnchorEntry&, const AnchorEntry&) const;
	};

	class RigidSortCriterion
	{
	public:
		bool operator()(const RigidEntry&, const RigidEntry&) const;
	};

	class ClumpStage : public IWorldStage
	{
	private:
		World* world;
		std::map<Anchor*, int> anchorSizeMap;
		std::map<Primitive*, PrimitiveSort> primitiveSizeMap;
		std::map<RigidJoint*, PrimitiveSort> rigidJointPowerMap;
		std::set<AnchorEntry, AnchorSortCriterion> anchors;
		std::set<RigidJoint*> rigidTwos;
		std::set<RigidEntry, RigidSortCriterion> rigidOnes;
		std::set<RigidJoint*> rigidZeros;
		std::set<PrimitiveEntry, PrimitiveSortCriterion> primitives;
		std::vector<MotorJoint*> motors;
		std::set<Clump*> anchoredClumps;
		std::set<Clump*> freeClumps;
		std::set<Assembly*> assemblies;
		std::set<Edge*> edges;
		std::set<MotorJoint*> motorAngles;
  
	private:
		Anchor* getBiggestAnchor();
		RigidJoint* getBiggestRigidOne();
		Primitive* getBiggestPrimitive();

		void anchorsInsert(Anchor*);
		void rigidTwosInsert(RigidJoint*);
		void rigidOnesInsert(RigidJoint*);
		void rigidZerosInsert(RigidJoint*);
		void primitivesInsert(Primitive*);
		void motorsInsert(MotorJoint*);
		void anchoredClumpsInsert(Clump*);
		void freeClumpsInsert(Clump*);
		void assembliesInsert(Assembly*);
		void edgesInsert(Edge*);
		void motorAnglesInsert(MotorJoint*);

		bool anchorsFind(Anchor*);
		bool rigidTwosFind(RigidJoint*);
		bool rigidOnesFind(RigidJoint*);
		bool rigidZerosFind(RigidJoint*);
		bool primitivesFind(Primitive*);
		bool motorsFind(MotorJoint*);
		bool anchoredClumpsFind(Clump*);
		bool freeClumpsFind(Clump*);
		bool assembliesFind(Assembly*);
		bool edgesFind(Edge*);
		bool motorAnglesFind(MotorJoint*);

		void anchorsErase(Anchor*);
		void rigidTwosErase(RigidJoint*);
		void rigidOnesErase(RigidJoint*);
		void rigidZerosErase(RigidJoint*);
		void primitivesErase(Primitive*);
		void motorsErase(MotorJoint*);
		void anchoredClumpsErase(Clump*);
		void freeClumpsErase(Clump*);
		void assembliesErase(Assembly*);
		void edgesErase(Edge*);
		void motorAnglesErase(MotorJoint*);

		void processAnchors();
		bool processRigidTwos();
		bool processRigidOnes();
		bool processRigidZeros();
		bool processPrimitives();
		void processMotors();
		void processAssemblies();
		void processEdges();
		void processMotorAngles();

		void addRigid(RigidJoint*);
		void removeRigid(RigidJoint*);

		void addMotor(MotorJoint*);
		void removeMotor(MotorJoint*);

		void addEdge(Edge*);
		void removeEdge(Edge*);

		void addAnchor(Anchor*);
		void removeAnchor(Anchor*);

		void addPrimitive(Primitive*);
		void removePrimitive(Primitive*);

		void destroyClumpGuts(Clump*);
		void destroyClump(Clump*);

		void destroyAssembly(Assembly*);
		void destroyInfluence(Influence*);

		void addAssemblyEdges(Assembly*);

		void removeExternalEdge(Edge*);
		void removeInternalEdge(Edge*);
		void removeAssemblyEdges(Assembly*);

		bool inClump(Primitive*);
		bool inBuffers(RigidJoint*);

		bool removeFromBuffers(RigidJoint*);
		void removeFromClump(Clump*, RigidJoint*);
		void removeSpanningTreeFast(Primitive*, RigidJoint*);
		void removeFromClumpFast(Primitive*, RigidJoint*);
		void removeFromAssemblyFast(Primitive*);

		void updateMotorJoint(MotorJoint*);

		bool validateAll();
		bool validateEdge(Edge*);
		bool validateRigid(RigidJoint*);
		bool validateMotor(MotorJoint*);
		bool validateExternal(Edge*);
		bool validatePrimitive(Primitive*);
		bool validateAnchor(Anchor*);
		bool validateClump(Clump*);
		bool validateAssembly(Assembly*);
		bool validateInfluence(Influence*);

		bool upToDate();
	public:
		//ClumpStage(const ClumpStage&);
		ClumpStage(IStage*, World*);
		virtual ~ClumpStage();
	public:
		virtual IStage::StageType getStageType()
		{
			return CLUMP_STAGE;
		}
		World* getWorld();
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		virtual int getMetric(IWorldStage::MetricType);
		void onPrimitiveAdded(Primitive*);
		void onPrimitiveRemoving(Primitive*);
		virtual void stepWorld(int, int, bool);
		void stepUi(int);
		void process();
		void onPrimitiveAddedAnchor(Primitive*);
		void onPrimitiveRemovingAnchor(Primitive*);
		void onPrimitiveCanSleepChanged(Primitive*);
		void onPrimitiveCanCollideChanged(Primitive*);
		void onMotorAngleChanged(MotorJoint*);
		//ClumpStage& operator=(const ClumpStage&);
  
	private:
		static PrimitiveSort getRigidPower(RigidJoint*);
	public:
		static PrimitiveSort getMotorPower(const MotorJoint*);
	private:
		static int numClumps(RigidJoint*);
	};
}

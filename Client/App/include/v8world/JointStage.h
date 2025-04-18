#pragma once
#include <set>
#include <map>
#include "v8world/IWorldStage.h"

namespace RBX
{
	class Joint;
	class Primitive;
	class ClumpStage;
	class Edge;
	class World;

	class JointStage : public IWorldStage
	{
	private:
		std::set<Joint*> incompleteJoints;
		std::multimap<Primitive*, Joint*> jointMap;
	  
	private:
		ClumpStage* getClumpStage();
		void moveEdgeToDownstream(Edge*);
		void removeEdgeFromDownstream(Edge*);
		void moveJointToDownstream(Joint*);
		void removeJointFromDownstream(Joint*);
		bool jointInList(Joint*);
		void removeFromList(Joint*);
		void insertToList(Joint*);
		bool pairInMap(Joint*, Primitive*);
		void insertToMap(Joint*, Primitive*);
		void removeFromMap(Joint*, Primitive*);
		bool edgeHasPrimitivesDownstream(Edge*);
	public:
		//JointStage(const JointStage&);
		JointStage(IStage*, World*);
		virtual ~JointStage();
	public:
		virtual IStage::StageType getStageType();
		virtual void onEdgeAdded(Edge*);
		virtual void onEdgeRemoving(Edge*);
		void onPrimitiveAdded(Primitive*);
		void onPrimitiveRemoving(Primitive*);
		void onJointPrimitiveNulling(Joint*, Primitive*);
		void onJointPrimitiveSet(Joint*, Primitive*);
		//JointStage& operator=(const JointStage&);
	};
}

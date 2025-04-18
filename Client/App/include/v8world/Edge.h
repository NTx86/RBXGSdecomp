#pragma once
#include "v8world/IPipelined.h"

namespace RBX
{
	class Primitive;

	namespace Sim
	{
		enum EdgeState
		{
			UNDEFINED,
			STEPPING,
			SLEEPING,
			TOUCHING,
			TOUCHING_SLEEPING
		};
	}

	class Edge : public IPipelined
	{
	public:
		enum EdgeType
		{
			JOINT,
			CONTACT
		};

	private:
		Sim::EdgeState edgeState;
		Primitive* prim0;
		Primitive* prim1;
		Edge* next0;
		Edge* next1;
		bool inEdgeList;
  
	public:
		//Edge(const Edge&);
		Edge(Primitive*, Primitive*);
		virtual ~Edge();

		bool getInEdgeList() const;
		void setInEdgeList(bool);
		virtual EdgeType getEdgeType() const;
		Sim::EdgeState getEdgeState() const;
		void setEdgeState(Sim::EdgeState);
		Primitive* getPrimitive(int i) const { return i == 0 ? prim0 : prim1; }
		Primitive* otherPrimitive(int) const;
		Primitive* otherPrimitive(const Primitive* p) const
		{
			return prim0 != p ? prim0 : prim1;
		}
		int getPrimitiveId(const Primitive*) const;
		Edge* getNext(const Primitive*) const;
		void setNext(Primitive*, Edge*);
		bool links(Primitive*, Primitive*) const;
		bool links(const Primitive* p) const
		{
			return p == prim0 || p == prim1;
		}
		virtual void setPrimitive(int, Primitive*);
		//Edge& operator=(const Edge&);
	};
}

#pragma once
#include "v8kernel/Connector.h"
#include "v8world/Edge.h"

namespace RBX
{
	class Contact : public Edge
	{
	private:
		int lastContactStep;
		int steppingIndex;
		float jointK;
		float elasticJointK;
		float kFriction;
	private:
		static bool ignoreBool;
	protected:
		static int contactPairMatches;
		static int contactPairMisses;
  
	private:
		virtual void putInKernel(Kernel*);
		virtual void removeFromKernel();
		virtual Edge::EdgeType getEdgeType() const;
	protected:
		Body* getBody(int) const;
		ContactConnector* createConnector();
		void deleteConnector(ContactConnector*&);
		virtual void deleteAllConnectors();
		virtual bool stepContact();
	public:
		//Contact(const Contact&);
		Contact(Primitive*, Primitive*);
		virtual ~Contact();
	public:
		int& steppingIndexFunc()
		{
			return steppingIndex;
		}
		virtual bool computeIsColliding(float);
		bool computeIsAdjacent(float);
		void onPrimitiveContactParametersChanged();
		bool step(int);
		//Contact& operator=(const Contact&);
  
	public:
		static bool isContact(Edge*);
	};
}

#pragma once
#include "v8kernel/KernelIndex.h"
#include "v8kernel/KernelInput.h"
#include "v8kernel/Body.h"
#include "v8kernel/Pair.h"
#include "v8kernel/Point.h"
#include "util/Math.h"
#include <G3DAll.h>

namespace RBX
{
	class Connector : public RBX::KernelIndex
	{
	public: // this is meant to be private
		__declspec(noinline) int& getKernelIndex() {return kernelIndex;}
	public:
		Connector(const Connector&);
		Connector() {};
		virtual ~Connector() {}
		virtual void computeForce(const float dt, bool throttling) {}
		virtual bool canThrottle() {return false;}
		virtual bool getBroken() {return false;}
		virtual float potentialEnergy() {return 0;};
		RBX::Connector& operator=(const RBX::Connector&);
	};

	class ContactConnector : public Connector
	{
	protected:
		RBX::GeoPair geoPair;
		float k;
		float kFriction;
		float kNeg;
		float firstApproach;
		float threshold;
		float forceMagLast;
		G3D::Vector3 frictionOffset;
	public:
		ContactConnector(const ContactConnector& other);
		ContactConnector::ContactConnector(float _k, float _kNeg, float _kFriction)
			: geoPair()
		{
			this->k = _k;
			this->kNeg = _kNeg;
			this->kFriction = _kFriction;
			this->frictionOffset = G3D::Vector3::zero();
			this->firstApproach = 0;
			this->threshold = 0;
			this->forceMagLast = 0;
		}

		void reset();
		void setBallBall(RBX::Body*, RBX::Body*, float, float);
		void setBallBlock(RBX::Body*, RBX::Body*, float, const G3D::Vector3*, RBX::NormalId, RBX::GeoPairType);
		void setPointPlane(RBX::Body*, RBX::Body*, const G3D::Vector3*, const G3D::Vector3*, int, RBX::NormalId);
		void setEdgeEdgePlane(RBX::Body*, RBX::Body*, const G3D::Vector3*, const G3D::Vector3*, RBX::NormalId, RBX::NormalId, RBX::NormalId, float);
		void setEdgeEdge(RBX::Body*, RBX::Body*, const G3D::Vector3*, const G3D::Vector3*, RBX::NormalId, RBX::NormalId);
		bool match(RBX::Body*, RBX::Body*, RBX::GeoPairType, int, int);
		virtual void computeForce(const float, bool);
		virtual bool canThrottle() const;
		virtual ~ContactConnector() {};
		RBX::ContactConnector& operator=(const RBX::ContactConnector&);
	};

	class PointToPointBreakConnector : public Connector
	{
	protected:
		Point* point0;
		Point* point1;
		float k;
		float breakForce;
		bool broken;
	protected:
		void forceToPoints(const G3D::Vector3&);
	public:
		//PointToPointBreakConnector(const PointToPointBreakConnector&);
		// TODO:: check if the ctor matches
		PointToPointBreakConnector(Point* _point0, Point* _point1, float _k, float _breakForce)
			: point0(_point0),
			  point1(_point1),
			  k(_k),
			  breakForce(_breakForce),
			  broken(false)
		{}

		virtual void computeForce(const float dt, bool throttling);
		virtual bool getBroken() {return this->broken;}
		virtual float potentialEnergy();
		void setBroken() {this->broken = true;}
		virtual ~PointToPointBreakConnector() {};
		//PointToPointBreakConnector& operator=(const PointToPointBreakConnector&);
	};

	class NormalBreakConnector : public PointToPointBreakConnector
	{
	private:
		NormalId normalIdBody0;

	public:
		//NormalBreakConnector(const NormalBreakConnector&);
		NormalBreakConnector(Point* _point0, Point* _point1, float _k, float _breakForce, NormalId _normalIdBody0)
			: PointToPointBreakConnector(_point0, _point1, _k, _breakForce),
			  normalIdBody0(_normalIdBody0)
		{
		}
		virtual ~NormalBreakConnector() {}
	public:
		virtual void computeForce(const float dt, bool throttling);
		//NormalBreakConnector& operator=(const NormalBreakConnector&);
	};

	class RotateConnector : public Connector
	{
	private:
		float k;
		Point* base0;
		Point* ray0;
		Point* ref0;
		Point* ref1;
		float lastRotation;
		int windings;
		KernelInput kernelInput;
	private:
		void computeParams(G3D::Vector3& normal, float& rotation, float& rotVel);
	public:
		RotateConnector(const RotateConnector& other);
		RotateConnector(Point* base0, Point* ray0, Point* ref0, Point* ref1, float kValue, float armLength);
		KernelInput* getKernelInput() {return &kernelInput;}
		virtual void computeForce(const float dt, bool throttling);
		virtual ~RotateConnector() {}
		RBX::RotateConnector& operator=(const RotateConnector& other);
	};
}
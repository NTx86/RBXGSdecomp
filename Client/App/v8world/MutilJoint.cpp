#include "v8world/MultiJoint.h"
#include "v8world/Primitive.h"

namespace RBX
{
	void MultiJoint::init(int numBreaking)
	{
		numBreakingConnectors = numBreaking;
		numConnector = 0;

		for (int i = 0; i < 4; i++)
		{
			point[(i*2)] = NULL;
			point[(i*2)+1] = NULL;
			connector[i] = NULL;
		}
	}

	MultiJoint::MultiJoint(int numBreaking)
		: Joint()
	{
		init(numBreaking);
	}

	MultiJoint::MultiJoint(
		Primitive* p0,
		Primitive* p1,
		const G3D::CoordinateFrame& jointCoord0,
		const G3D::CoordinateFrame& jointCoord1,
		int numBreaking)
		: Joint(p0, p1, jointCoord0, jointCoord1)
	{
		init(numBreaking);
	}

	MultiJoint::~MultiJoint()
	{
		RBXAssert(connector[0] == NULL);
		RBXAssert(numConnector == 0);
	}

	void MultiJoint::putInKernel(Kernel* _kernel)
	{
		IPipelined::putInKernel(_kernel);
	}

	void MultiJoint::addToMultiJoint(Point* point0, Point* point1, Connector* _connector)
	{
		RBXAssert(numConnector < 4);

		point[(numConnector*2)] = point0;
		point[(numConnector*2)+1] = point1;
		connector[numConnector] = _connector;

		getKernel()->insertConnector(_connector);
		++numConnector;
	}

	Point* MultiJoint::getPoint(int id)
	{
		RBXAssert(id < 8);
		RBXAssert(point[id]);
		return point[id];
	}

	void MultiJoint::removeFromKernel()
	{
		RBXAssert(this->inKernel());

		for (int i = 0; i < numConnector; i++)
		{
			RBXAssert(point[(i*2)]);
			RBXAssert(point[(i*2)+1]);

			getKernel()->deletePoint(point[(i*2)]);
			getKernel()->deletePoint(point[(i*2)+1]);
			point[(i*2)] = NULL;
			point[(i*2)+1] = NULL;

			RBXAssert(connector[i]);

			getKernel()->removeConnector(connector[i]);
			delete connector[i];
			connector[i] = NULL;
		}

		numConnector = 0;

		IPipelined::removeFromKernel();
		RBXAssert(!this->inKernel());
	}

	bool MultiJoint::isBroken() const
	{
		RBXAssert(numBreakingConnectors <= numConnector);
		RBXAssert(this->inKernel());

		for (int i = 0; i < numBreakingConnectors; i++)
		{
			RBXAssert(connector[i]);
			if (connector[i]->getBroken())
				return true;
		}

		return false;
	}

	float MultiJoint::getJointK()
	{
		float k1 = getPrimitive(1)->getJointK();
		float k0 = getPrimitive(0)->getJointK();

		return G3D::min(k0, k1);
	}
}
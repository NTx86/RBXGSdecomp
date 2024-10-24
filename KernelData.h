#pragma once
#include "Body.h"
#include "Point.h"
#include "Connector.h"
#include "IndexArray.h"

namespace RBX {
	class KernelData
	{
		public:
			KernelData::~KernelData()
			{
				RBXAssert(!points.size());
				RBXAssert(!bodies.size());
				RBXAssert(!connectors.size());
				RBXAssert(!connectors2ndPass.size());
			}
			IndexArray<Body, &Body::getKernelIndex> bodies;
			IndexArray<Point, &Point::getKernelIndex> points;
			IndexArray<Connector, &Connector::getKernelIndex> connectors;
			IndexArray<Connector, &Connector::getKernelIndex> connectors2ndPass;
	};
}
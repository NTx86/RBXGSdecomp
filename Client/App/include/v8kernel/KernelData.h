#pragma once
#include "v8kernel/Body.h"
#include "v8kernel/Point.h"
#include "v8kernel/Connector.h"
#include "util/IndexArray.h"

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
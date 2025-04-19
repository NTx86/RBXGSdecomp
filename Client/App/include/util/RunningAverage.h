#pragma once
#include "util/Quaternion.h"

namespace RBX
{
	class RunningAverageState
	{
	private:
		G3D::Vector3 position;
		Quaternion angles;

	public:
		//RunningAverageState(const RunningAverageState&);
		RunningAverageState();
		void update(const G3D::CoordinateFrame&, float);
		bool withinTolerance(const G3D::CoordinateFrame&, float, float);
		//RunningAverageState& operator=(const RunningAverageState&);

	private:
		static float weight();
	public:
		static int stepsToSleep();
	};
}

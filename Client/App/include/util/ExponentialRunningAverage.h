#include <g3d/Vector3.h>

namespace RBX
{
	class floatERA
	{
		float weight;
		float avg;

	public:
		floatERA(float);
		floatERA();

		void reset();
		float pushAndGetAverage(float value);
		float getAverage();
	};

	class Vector3ERA
	{
		float weight;
		float x, y, z;

	public:
		Vector3ERA(float);
		Vector3ERA();

		void reset();
		G3D::Vector3 pushAndGetAverage(G3D::Vector3 value);
		G3D::Vector3 getAverage();
	};
}
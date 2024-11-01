#include "util/Units.h"

using namespace RBX;

G3D::Vector3 Units::kmsAccelerationToRbx(const G3D::Vector3& kmsAccel)
{
	return Vector3(kmsAccel.x * 20.0f, kmsAccel.y * 20.0f, kmsAccel.z * 20.0f);
}
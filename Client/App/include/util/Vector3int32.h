#pragma once
#include <G3DAll.h>

namespace RBX
{
	class Vector3int32
	{
	public:
		int x;
		int y;
		int z;
 
	public:
		Vector3int32() : x(0), y(0), z(0) {}
		Vector3int32(int x, int y, int z) : x(x), y(y), z(z) {}

	public:
		int& operator[](int i) { return ((int*)this)[i]; }
		const int& operator[](int i) const { return ((int*)this)[i]; }

		Vector3int32 operator+(const Vector3int32& other) const
		{
			return Vector3int32(
				this->x + other.x,
				this->y + other.y,
				this->z + other.z
			);
		}

		bool operator==(const Vector3int32& other) const
		{
			return 
				this->x == other.x &&
				this->y == other.y &&
				this->z == other.z;
		}

		bool operator!=(const Vector3int32& other) const
		{
			return 
				this->x != other.x ||
				this->y != other.y ||
				this->z != other.z;
		}

		Vector3 toVector3() const { return Vector3(x,y,z); }

		static Vector3int32 floor(const Vector3& v)
		{
			return Vector3int32(
				G3D::iRound(::floor(v.x)),
				G3D::iRound(::floor(v.y)),
				G3D::iRound(::floor(v.z))
			);
		}
	};
}

#pragma once

namespace RBX
{
	enum SurfaceType
	{
		NO_SURFACE,
		GLUE,
		WELD,
		STUDS,
		INLET,
		SPAWN_deprecated,
		ROTATE,
		ROTATE_V,
		ROTATE_P,
		NUM_SURF_TYPES
	};
}

template <typename T> 
class Vector6
{
private:
	T data[6];
public:
	Vector6() 
	{
		for (int i = 5; i > -1; i--)
		{
			data[i] = (T)0;
		}
	}
	T& operator[](int index);

	const T& operator[](int) const;
};
#pragma once
#include "v8world/Controller.h"

namespace RBX
{
	class SurfaceData
	{
	public:
		Controller::InputType inputType;
		float paramA;
		float paramB;

	public:
		SurfaceData();
		bool operator==(const SurfaceData&) const;
		bool isEmpty() const;

	public:
		static const SurfaceData& empty();
	};
}

#pragma once

namespace RBX
{
	class Primitive;

	class Anchor
	{
	private:
		Primitive* primitive;

	public:
		Anchor(Primitive*);
		~Anchor();
		Primitive* getPrimitive();
	};
}

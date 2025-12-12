#include "reflection/type.h"
#include <g3d/coordinateframe.h>

namespace RBX
{
	namespace Reflection
	{
		// TODO: check if type singletons are matching
		template<>
		const Type& Type::singleton<G3D::CoordinateFrame>()
		{
			static Type type("CoordinateFrame", typeid(G3D::CoordinateFrame));
			return type;
		}
	}
}

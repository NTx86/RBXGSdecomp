#include "reflection/type.h"

namespace RBX
{
	namespace Reflection
	{
		// TODO: check if type singletons are matching
		template<>
		const Type& Type::singleton<void>()
		{
			static Type type("void", typeid(void));
			return type;
		}

		// TODO: check ctor match
		Value::Value()
			: _type(&Type::singleton<void>()),
			  value(NULL)
		{
		}
	}
}

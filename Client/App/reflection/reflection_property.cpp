#include "reflection/enumconverter.h"
#include "reflection/property.h"
#include "reflection/object.h"

namespace RBX
{
	namespace Reflection
	{
		// TODO: check if type singletons are matching
		template<>
		const Type& Type::singleton<const PropertyDescriptor*>()
		{
			static Type type("Property", typeid(const PropertyDescriptor*));
			return type;
		}

		PropertyDescriptor::PropertyDescriptor(ClassDescriptor& classDescriptor, const Type& type, const char* name, const char* category, Functionality flags)
			: MemberDescriptor(classDescriptor, name, category),
			  bIsPublic((flags & UI) != 0),
			  bCanStreamWrite((flags & STREAMING) != 0),
			  type(type)
		{
			classDescriptor.PropertyContainer::declare(this);
		}

		std::vector<const EnumDescriptor*>& EnumDescriptor::allEnums()
		{
			static std::vector<const EnumDescriptor*> s;
			return s;
		}

		EnumDescriptor::EnumDescriptor(const char* typeName, const type_info& type)
			: Type(typeName, type, "token"),
			  allItems(),
			  enumCount(0),
			  enumCountMSB(0)
		{
			allEnums().push_back(this);
		}

		EnumDescriptor::~EnumDescriptor()
		{
			RBXASSERT(std::find(allEnums().begin(), allEnums().end(), this) != allEnums().end());
			allEnums().erase(std::find(allEnums().begin(), allEnums().end(), this));
		}
	}
}

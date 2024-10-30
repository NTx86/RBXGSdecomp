#include "util/Debug.h"
#include <typeinfo>
using namespace RBX;

bool doCrash = true;
Debugable::AssertAction Debugable::assertAction = IgnoreAssert;

void Debugable::dump(std::ostream& stream)
{
	const std::type_info& type = typeid(*this);
	stream << type.name();
}

void Debugable::doCrash() {
	if (::doCrash)
	{
		int* nullPtr1 = (int*)0;
		int* nullPtr2 = (int*)4;
		*nullPtr1 = *nullPtr2;
	}
}
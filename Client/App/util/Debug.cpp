#include "util/Debug.h"
using namespace RBX;

bool doCrash = true;
Debugable::AssertAction Debugable::assertAction = IgnoreAssert;

void Debugable::PlaceHolder() { //vtable temporary
	return;
}

void Debugable::doCrash() {
	if (::doCrash)
	{
		int* nullPtr1 = (int*)0;
		int* nullPtr2 = (int*)4;
		*nullPtr1 = *nullPtr2;
	}
}
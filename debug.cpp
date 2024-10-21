#include "debug.h"
using namespace RBX;

bool DoCrash = true; //this varialbe is suppose to be have the same name as the function, but that causes issues
Debugable::AssertAction Debugable::assertAction = IgnoreAssert;

void Debugable::PlaceHolder() { //vtable temporary
	return;
}

void Debugable::doCrash() {
	if (DoCrash)
	{
		int* nullPtr1 = (int*)0;
		int* nullPtr2 = (int*)4;
		*nullPtr1 = *nullPtr2;
	}
}
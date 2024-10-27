// RBXGSdecomp.cpp : Defines the entry point for the DLL application.
//
//#include "Kernel.h"
#include <windows.h>
#include <boost/shared_ptr.hpp>

//using namespace boost;

void TestFunction()
{

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	//TestFunction();
    return TRUE;
}


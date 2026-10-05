#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "HookUtil.h"
#include "v8world/World.h"
#include "v8world/ClumpStage.h"
#include "v8world/Primitive.h"
#include "v8world/Contact.h"
#include "v8world/ContactManager.h"

#define RBXWorldCtor 0x0053F680
#define RBXWorldstep 0x0053ED40
#define RBXWorldcomputeFallen 0x0053E3F0
#define RBXClumpStageProcess 0x005A3680
#define RBXPrimitiveNewGeometry 0x0052CE70
#define RBXContactManagercreateContact 0x0058D680
//unneeded
#define RBXContactBallBallContactCtor 0x0058C790
#define RBXContactBallBlockContactCtor 0x0058C8A0
#define RBXContactBlockBlockContactCtor 0x0058D520


void __fastcall worldCtor(RBX::World* AThis)
{
	free(AThis);
	AThis = new RBX::World();
}

void __fastcall BallBallContactCtor(RBX::BallBallContact* AThis, void* EDX, RBX::Primitive* p0, RBX::Primitive* p1)
{
	free(AThis);
	AThis = new RBX::BallBallContact(p0, p1);
}

void __fastcall BallBlockContactCtor(RBX::BallBlockContact* AThis, void* EDX, RBX::Primitive* p0, RBX::Primitive* p1)
{
	free(AThis);
	AThis = new RBX::BallBlockContact(p0, p1);
}

void __fastcall BlockBlockContactCtor(RBX::BlockBlockContact* AThis, void* EDX, RBX::Primitive* p0, RBX::Primitive* p1)
{
	free(AThis);
	AThis = new RBX::BlockBlockContact(p0, p1);
}

RBX::Geometry* HooknewGeometry(RBX::Geometry::GeometryType geometryType)
{
	switch (geometryType)
	{
		case RBX::Geometry::GEOMETRY_BLOCK: return new RBX::Block;
		case RBX::Geometry::GEOMETRY_BALL: return new RBX::Ball;
		default: return RBX::Geometry::nullGeometry();
	}
}

class MainHookClass
{
	public:
		MainHookClass() {}
		void doHooks();
};

void MainHookClass::doHooks()
{
	//World::World
	HookFunc((void*)RBXWorldCtor, worldCtor);

	//ClumpStage::process
	void (RBX::ClumpStage:: * ClumpStageProcessPtr)(void) = &RBX::ClumpStage::process;
	void* ClumpStageProcessAddr = *(void**)(&ClumpStageProcessPtr);
	HookFunc((void*)RBXClumpStageProcess, ClumpStageProcessAddr);

	//World::step
	float (RBX::World:: * WorldstepPtr)(float) = &RBX::World::step;
	void* WorldstepPtrAddr = *(void**)(&WorldstepPtr);
	HookFunc((void*)RBXWorldstep, WorldstepPtrAddr);

	//RBX::World::computeFallen
	void (RBX::World:: * WorldcomputeFallenPtr)(G3D::Array<RBX::Primitive*>&) const = &RBX::World::computeFallen;
	void* WorldcomputeFallenAddr = *(void**)(&WorldcomputeFallenPtr);
	HookFunc((void*)RBXWorldcomputeFallen, WorldcomputeFallenAddr);

	//RBX::Primitive::NewGeometry
	HookFunc((void*)RBXPrimitiveNewGeometry, RBX::Primitive::newGeometry);

	//RBX::ContactManager::createContact ContactManagercreateContact
	RBX::Contact* (RBX::ContactManager:: * ContactManagercreateContactPtr)(RBX::Primitive* p0, RBX::Primitive* p1) = &RBX::ContactManager::createContact;
	void* ContactManagercreateContactAddr = *(void**)(&ContactManagercreateContactPtr);
	HookFunc((void*)RBXContactManagercreateContact, ContactManagercreateContactAddr);
}

extern "C" __declspec(dllexport) DWORD WINAPI MainThread(LPVOID param)
{
	MainHookClass hookclass;
	hookclass.doHooks();
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		//HttpGet = (_HttpGet)HookTramp((void*)RBXHttpGetAddr, HttpGetHook, 5); 
		break;
	default:
		break;
	}
	return TRUE;
}
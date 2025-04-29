#pragma once
#include <ostream>

namespace RBX {
	class Debugable {
		public:
			virtual void dump(std::ostream& stream);
			static void doCrash();
			enum AssertAction
			{
			  CrashOnAssert = 0x0,
			  IgnoreAssert = 0x1,
			};
			static AssertAction assertAction;
	};
}

//#define RBXASSERT(expr) if ( RBX::Debugable::assertAction == Debugable::CrashOnAssert && !(expr)) RBX::Debugable::doCrash()
// copied from assert.h
//#define RBXASSERT(expr) (void)( ( RBX::Debugable::assertAction != RBX::Debugable::CrashOnAssert || !!(expr) ) || (RBX::Debugable::doCrash(), 0) )
#define SCOPED(expr) do \
	{ \
		expr; \
	} \
	while (0)
#define RBXASSERT(expr) SCOPED( (void)( ( RBX::Debugable::assertAction != RBX::Debugable::CrashOnAssert || !!(expr) ) || (RBX::Debugable::doCrash(), 0) ) )
//temporary redefinition in case this is being implemented while new code is being decompiled.
#define RBXAssert(expr) RBXASSERT(expr)

template <typename To, typename From>
To rbx_static_cast(From u)
{
	RBXASSERT(dynamic_cast<To>(u) == static_cast<To>(u));
	return static_cast<To>(u);
}

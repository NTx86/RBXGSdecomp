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

//#define RBXAssert(expr) if ( RBX::Debugable::assertAction == Debugable::CrashOnAssert && !(expr)) RBX::Debugable::doCrash()
// copied from assert.h
//#define RBXAssert(expr) (void)( ( RBX::Debugable::assertAction != RBX::Debugable::CrashOnAssert || !!(expr) ) || (RBX::Debugable::doCrash(), 0) )
#define RBXAssert(expr) do \
    { \
		(void)( ( RBX::Debugable::assertAction != RBX::Debugable::CrashOnAssert || !!(expr) ) || (RBX::Debugable::doCrash(), 0) ); \
    } \
    while(0)

template <typename To, typename From>
To rbx_static_cast(From u)
{
	RBXAssert(dynamic_cast<To>(u) == static_cast<To>(u));
	return static_cast<To>(u);
}

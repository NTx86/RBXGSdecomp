#pragma once
#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "util/Name.h"
#include "util/Debug.h"

namespace RBX
{
	namespace Reflection
	{
		// TODO: MemberDescriptorContainer<PropertyDescriptor>::declare and MemberDescriptorContainer<FunctionDescriptor>::declare use different sync functions, make sure thats happening
		// rename to "sync" later too
		static boost::recursive_mutex& sync_0()
		{
			static boost::recursive_mutex s;
			return s;
		}

		class Descriptor : public boost::noncopyable
		{
		public:
			const Name& name;
	  
		public:
			//Descriptor(const Descriptor&);
			Descriptor(const Name&);
			Descriptor(const char* name)
				: name(Name::declare(name, -1))
			{
				RBXASSERT(!this->name.empty());
			}
			virtual ~Descriptor() {}
		public:
			//Descriptor& operator=(const Descriptor&);
		};
	}
}

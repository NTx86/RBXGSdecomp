#pragma once
#include <g3d/array.h>
#include <boost/noncopyable.hpp>
#include "util/Debug.h"
#include "v8kernel/Connector.h"

namespace RBX {
	template <typename tInstance>
	class IndexArrayOld : boost::noncopyable_::noncopyable
	{
		public:
			G3D::Array<tInstance *> array;
			inline tInstance* operator[](int index){ return array[index]; }
			inline int size() {return array.size();}
	};

	template <typename tInstance, int& (tInstance::*getIndex)()>
	class IndexArray : boost::noncopyable_::noncopyable
	{
		private:
			int& indexOf(tInstance* instance) const
			{
				return (instance->*getIndex)();
			}
		public:
			G3D::Array<tInstance *> array;
			inline tInstance* operator[](int index){ 
				RBXAssert(indexOf(array[index]) == index);
				return array[index]; 
			}

			inline int size() {return array.size();}

			__declspec(noinline) void fastRemove(tInstance* instance)
			{
				RBXAssert(!instance);
			}

			void fastAppend(tInstance* instance)
			{
				RBXAssert(instance);
				RBXAssert(indexOf(instance) == -1);
				indexOf(instance) = array.size();
				array.append(instance);
			}

	};
}
#pragma once
#include <g3d/array.h>
#include <boost/noncopyable.hpp>
#include "util/Debug.h"

namespace RBX {
	template <typename tInstance, int& (tInstance::*getIndex)()>
	class IndexArray : boost::noncopyable
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

			inline int size() const {return array.size();}

			// 100% match if G3D::Array<>::find has __declspec(noinline)
			void fastRemove(tInstance* item)
			{
				RBXAssert(array.find(item) != array.end());

				int removeIndex = indexOf(item);
				RBXAssert(removeIndex >= 0);
				RBXAssert(array[removeIndex] == item);

				// this does something similar to G3D::Array<>::fastRemove
				tInstance* movedItem = array[size() - 1];
				array[removeIndex] = movedItem;
				indexOf(movedItem) = removeIndex;
				array.resize(array.size() - 1, false);

				indexOf(item) = -1;
			}

			void fastAppend(tInstance* item)
			{
				RBXAssert(item);
				RBXAssert(indexOf(item) == -1);
				indexOf(item) = array.size();
				array.append(item);
			}

	};
}
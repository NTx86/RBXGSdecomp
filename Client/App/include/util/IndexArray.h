#pragma once
#include <g3d/array.h>
#include <boost/noncopyable.hpp>
#include "util/Debug.h"

namespace RBX
{
	template <typename tInstance, int& (tInstance::*getIndex)()>
	class IndexArray : public boost::noncopyable
	{
	private:
		G3D::Array<tInstance*> array;

	private:
		int& indexOf(tInstance* instance) const
		{
			return (instance->*getIndex)();
		}

	public:
		void fastAppend(tInstance* item)
		{
			RBXASSERT(item);
			RBXASSERT(indexOf(item) == -1);
			indexOf(item) = array.size();
			array.append(item);
		}

		void fastRemove(tInstance* item)
		{
			RBXASSERT(array.find(item) != array.end());

			int removeIndex = indexOf(item);
			RBXASSERT(removeIndex >= 0);
			RBXASSERT(array[removeIndex] == item);

			// this does something similar to G3D::Array<>::fastRemove
			tInstance* movedItem = array[size() - 1];
			array[removeIndex] = movedItem;
			indexOf(movedItem) = removeIndex;
			array.resize(array.size() - 1, false);

			indexOf(item) = -1;
		}

		bool fastContains(tInstance* item) const;

		const G3D::Array<tInstance*>& underlyingArray() const
		{
			return array;
		}

		tInstance* operator[](G3D::uint32 index) const
		{ 
			RBXASSERT(indexOf(array[index]) == index);
			return array[index]; 
		}

		tInstance* operator[](int index) const
		{ 
			RBXASSERT(indexOf(array[index]) == index);
			return array[index]; 
		}

		tInstance* operator[](G3D::uint32 index)
		{ 
			RBXASSERT(indexOf(array[index]) == index);
			return array[index]; 
		}

		tInstance* operator[](int index)
		{ 
			RBXASSERT(indexOf(array[index]) == index);
			return array[index]; 
		}

		int size() const
		{
			return array.size();
		}
	};
}

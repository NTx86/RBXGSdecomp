#pragma once
#include <vector>
#include <util/debug.h>

namespace RBX
{
	template<typename T> unsigned int fastRemoveShort(std::vector<T>& vec, T const& item)
	{
		std::vector<T>::iterator iter = std::find(vec.begin(), vec.end(), item);
		unsigned int answer = (unsigned int)std::distance(vec.begin(),iter);
		RBXAssert(vec[answer] == item);
		RBXAssert(iter != vec.end());
		RBXAssert(vec.size() < 32);
		std::vector<T>::iterator vecEnd = vec.end();
		vecEnd--;
		RBXAssert(*vecEnd == *(vec.end()-1)); //useless check, in 2013 this either got removed or optimized out
		if (iter != vecEnd) {
			*iter = *vecEnd;
		}
		vec.resize(vec.size()-1);
		return answer;
	}
}
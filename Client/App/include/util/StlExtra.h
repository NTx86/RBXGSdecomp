#pragma once
namespace RBX
{
	//Unfinished!!!
	template<typename T> unsigned int fastRemoveShort(std::vector<T>& vec, T const& item);
	/*{
		std::vector<T>::iterator iter;
		for (iter = vec.begin(); iter != vec.end(); iter++)
		{
			if (*iter == item)
				break;
		}
		unsigned int index = std::distance(vec.begin(),iter);
		RBXAssert(vec[index] == item);
		RBXAssert(iter == vec.end());
		RBXAssert(vec.size() < 32);
		if (iter != vec.end() - 1) {
			*iter = vec.back();
		}
		vec.pop_back();
		return index;
	}*/
}
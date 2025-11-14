#pragma once

template <typename T> 
class Vector6
{
private:
	T data[6];
public:
	Vector6()
	{
		//TODO: Is this the best way to handle this?
		for (int i = 5; i > -1; i--)
		{
			data[i] = (T)0;
		}
	}
	T& operator[](int index);

	const T& operator[](int) const;
};
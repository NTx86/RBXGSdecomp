#pragma once

namespace RBX
{
	template<typename T, typename U>
	class ComputeProp
	{
	private:
		T val;
		bool dirty;
		U* object;
		T(U::* getFunc)();

	public:
		ComputeProp(U*, T(U::*)());
		T getValue() const;
		operator T() const;
		const T* getValuePointer() const;
		const T& getValueRef() const;
		bool setDirty() const;
	};
}

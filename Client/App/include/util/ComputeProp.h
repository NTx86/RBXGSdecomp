#pragma once

namespace RBX
{
	// NOTE: i had to add a `const` to the getFunc definition
	template<typename T, typename U>
	class ComputeProp
	{
	private:
		mutable T val;
		mutable bool dirty;
		U* object;
		T(U::* getFunc)() const;

	public:
		ComputeProp(U* object, T(U::* getFunc)() const)
			: dirty(true),
			  object(object),
			  getFunc(getFunc)
		{
		}
		T getValue() const;
		operator T() const
		{
			if (dirty)
			{
				val = (object->*getFunc)();
				dirty = false;
			}
			return val;
		}
		const T* getValuePointer() const;
		const T& getValueRef() const;
		bool setDirty() const
		{
			dirty = true;
			return true; // TODO: this is a placeholder return. figure out the true value.
		}
	};
}

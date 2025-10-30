#pragma once
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace RBX
{
	template<typename T>
	class CopyOnWrite : public boost::noncopyable
	{
	private:
		boost::shared_ptr<T> object;
	  
	public:
		//CopyOnWrite(const CopyOnWrite&);
		CopyOnWrite(const T&);
		CopyOnWrite();
	public:
		//T** operator class T *boost::shared_ptr<T>() const; // TODO: is this definition right? (no it isnt, fix the error!!!)
		const T& operator*() const
		{
			return *object.get();
		}
		const T* operator->() const
		{
			return object.get();
		}
		boost::shared_ptr<const T> read() const
		{
			return object;
		}
		boost::shared_ptr<T>& write();
		void reset();
	public:
		~CopyOnWrite();
	public:
		//CopyOnWrite& operator=(const CopyOnWrite&);
	};

	// Function traits
	template<typename T>
	struct FunctionTraits;

	template<typename ReturnType>
	struct FunctionTraits<ReturnType()>
	{
		static const int ArgCount = 0;
		typedef ReturnType ReturnType;
	};

	template<typename ReturnType, typename Arg1>
	struct FunctionTraits<ReturnType(Arg1)>
	{
		static const int ArgCount = 1;
		typedef ReturnType ReturnType;
		typedef Arg1 Arg1Type;
	};

	template<typename ReturnType, typename Arg1, typename Arg2>
	struct FunctionTraits<ReturnType(Arg1, Arg2)>
	{
		static const int ArgCount = 2;
		typedef ReturnType ReturnType;
		typedef Arg1 Arg1Type;
		typedef Arg2 Arg2Type;
	};

	template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3>
	struct FunctionTraits<ReturnType(Arg1, Arg2, Arg3)>
	{
		static const int ArgCount = 3;
		typedef ReturnType ReturnType;
		typedef Arg1 Arg1Type;
		typedef Arg2 Arg2Type;
		typedef Arg3 Arg3Type;
	};

	template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	struct FunctionTraits<ReturnType(Arg1, Arg2, Arg3, Arg4)>
	{
		static const int ArgCount = 4;
		typedef ReturnType ReturnType;
		typedef Arg1 Arg1Type;
		typedef Arg2 Arg2Type;
		typedef Arg3 Arg3Type;
		typedef Arg4 Arg4Type;
	};

	template<typename ReturnType, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	struct FunctionTraits<ReturnType(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
		static const int ArgCount = 5;
		typedef ReturnType ReturnType;
		typedef Arg1 Arg1Type;
		typedef Arg2 Arg2Type;
		typedef Arg3 Arg3Type;
		typedef Arg4 Arg4Type;
		typedef Arg5 Arg5Type;
	};
}

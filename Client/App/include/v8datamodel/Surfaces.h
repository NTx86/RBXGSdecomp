#pragma once
#include <boost/noncopyable.hpp>
#include "v8datamodel/Surface.h"
#include "util/Vector6.h"

namespace RBX
{
	class PartInstance;
	class Surfaces : public boost::noncopyable
	{
	private:
		Surface Top;
		Surface Bottom;
		Surface Left;
		Surface Right;
		Surface Front;
		Surface Back;
	  
	public:
		//Surfaces(const Surfaces&);
		Surfaces(PartInstance*);
	public:
		const Surface& operator[](NormalId) const;
		RBX::Surface& operator[](NormalId);
		Vector6<SurfaceType> surf6() const;
		const Reflection::PropertyDescriptor& getSurfaceType(NormalId) const;
		const Reflection::PropertyDescriptor& getSurfaceInput(NormalId) const;
		const Reflection::PropertyDescriptor& getParamA(NormalId) const;
		const Reflection::PropertyDescriptor& getParamB(NormalId) const;
		const bool isStandardPart() const;
	public:
		~Surfaces();
	public:
		//Surfaces& operator=(const Surfaces&);
	  
	public:
		static bool isSurfaceDescriptor(const Reflection::PropertyDescriptor&);
	};
}

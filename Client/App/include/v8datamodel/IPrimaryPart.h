#pragma once

namespace RBX
{
	class PartInstance;
	class IPrimaryPart
	{
	public:
		virtual PartInstance* getPrimaryPart() = 0;
		virtual const PartInstance* getPrimaryPartConst() const = 0;
	public:
		//IPrimaryPart(const IPrimaryPart&);
		IPrimaryPart();
	public:
		//IPrimaryPart& operator=(const IPrimaryPart&);
	};
}

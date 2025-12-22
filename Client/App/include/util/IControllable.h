#pragma once

namespace RBX
{
	class IControllable
	{
	public:
		//IControllable(const IControllable&);
		IControllable()
		{
		}
		virtual ~IControllable()
		{
		}
	public:
		virtual bool isControllable() const = 0;
	public:
		//IControllable& operator=(const IControllable&);
	};
}

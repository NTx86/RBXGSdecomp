#pragma once
#include <string>

namespace RBX
{
	class IMetric
	{
	public:
		//IMetric(const IMetric&);
		IMetric()
		{
		}
		virtual ~IMetric()
		{
		}
	public:
		virtual std::string getMetric(const std::string&) const = 0;
	public:
		//IMetric& operator=(const IMetric&);
	};
}

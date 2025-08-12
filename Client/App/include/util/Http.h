#pragma once
#include <string>
namespace RBX
{
	class Http 
	{
	public: 
		Http(const std::string* url);
	private:
		std::string url;
		std::string additionalHeaders;
	};
}
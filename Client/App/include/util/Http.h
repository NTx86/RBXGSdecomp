#pragma once
#include <string>
#include <windows.h>
#include <atlutil.h>
namespace RBX
{
	class Http 
	{
	public: 
		Http(const std::string* url);
		bool isRobloxSite(const char* url);
		bool isScript(const char* url);
		

		std::string url;
		std::string additionalHeaders;
	};
}
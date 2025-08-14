#pragma once
#include <string>
#include <windows.h>
#include <atlutil.h>
#include <atlstr.h>
namespace RBX
{
	class Http 
	{
	public: 
		Http(const std::string* url);
		static bool isRobloxSite(const char* url);
		static bool isScript(const char* url);
	private:
		std::string url;
		std::string additionalHeaders;
	};
}
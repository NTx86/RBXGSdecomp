#pragma once
#include <string>
#include <windows.h>
#include <atlutil.h>
namespace RBX
{
	class Http 
	{
	public:          
		Http(const std::string& url);
		Http(const char* url);
	public:          
		std::string url;
		std::string additionalHeaders;
	public:
		static bool isRobloxSite(const char* url);
		static bool isScript(const char* url);
		static bool trustCheck(char* url);
	};
}
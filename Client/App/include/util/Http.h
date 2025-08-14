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
		bool isRobloxSite(const char* url);
		bool isScript(const char* url);
		bool trustCheck(char* url);
	};
}
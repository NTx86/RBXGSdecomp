#pragma once
#include <string>
#include <windows.h>
#include <atlbase.h>
#include <atlutil.h>
#include <wincrypt.h> 
namespace RBX
{
	class Http 
	{
	public: 
		Http(const std::string* url);
		bool isRobloxSite(const wchar_t* url);
	private:
		std::string url;
		std::string additionalHeaders;
	};
}
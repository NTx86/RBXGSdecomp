#pragma once
#include <string>

namespace RBX
{
	class Http
	{
	public:
		const std::string url;
		std::string additionalHeaders;

	public:
		//Http(const Http&);
		Http(const std::string&);
		Http(const char*);
	public:
		void post(std::istream&, bool, std::string&);
		void get(std::string&);
	public:
		~Http();

	public:
		static bool trustCheck(const char*);
		static bool isScript(const char*);
		static bool isRobloxSite(const char*);
		static bool isCached(const char*);
	};
}

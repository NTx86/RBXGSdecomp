#include "util/Http.h"

namespace RBX
{
	Http::Http(const std::string* url) 
		:url(*url)
	{
	}

	bool isRobloxSite(const wchar_t* url)
	{
		ATL::CUrl targetUrl;
		targetUrl.CrackUrl(url);

		if ( targetUrl.GetScheme()
			&& (targetUrl.GetScheme() <= ATL_URL_SCHEME_GOPHER || targetUrl.GetScheme() > ATL_URL_SCHEME_HTTPS) )
		{
			return false;
		}

		LPCTSTR hostName = targetUrl.GetHostName();
		CStringA host(hostName);
		host.MakeLower();

		if (host.Right(10) == "roblox.com")
			return true;
		else if (host.Right(12) == "72.32.62.210")
			return true;
		else if (host.Right(16) == "robloxopolis.com")
			return true;

		return false;
	}
}
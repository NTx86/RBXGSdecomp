#include "util/Http.h"

namespace RBX
{
	Http::Http(const std::string* url) 
		:url(*url)
	{
	}

	bool isRobloxSite(const char* url)
	{
		// i genuinely hate windows APIs and datatypes
		ATL::CUrl targetUrl;
		targetUrl.CrackUrl(url); 

		if ( targetUrl.GetScheme()
			&& (targetUrl.GetScheme() <= ATL_URL_SCHEME_GOPHER || targetUrl.GetScheme() > ATL_URL_SCHEME_HTTPS) )
		{
			return false;
		}

		CString hostName = targetUrl.GetHostName();
		hostName.MakeLower();

		if (hostName.Right(10) == "roblox.com")
			return true;
		else if (hostName.Right(12) == "72.32.62.210")
			return true;
		else if (hostName.Right(16) == "robloxopolis.com")
			return true;

		return false;
	}

	bool isScript(const char* url)
	{
		ATL::CUrl targetUrl;
		targetUrl.CrackUrl(url);

		if(targetUrl.GetScheme() == ATL_URL_SCHEME_FTP 
			|| targetUrl.GetScheme() > ATL_URL_SCHEME_GOPHER && targetUrl.GetScheme() <= ATL_URL_SCHEME_HTTPS )
		{
			return false;
		}

		CString host(url);

		if(host.Find("javascript:") == 0)
			return true;
		if(host.Find("jscript:") == 0)
			return true;

		return false;
	}
}
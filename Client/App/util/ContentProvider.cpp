#include "util/ContentProvider.h"

namespace RBX
{
	// TODO: check match
	bool operator<(const ContentId& a, const ContentId& b)
	{
		return a.toString() < b.toString();
	}

	ContentProvider& ContentProvider::singleton()
	{
		static ContentProvider sing;
		return sing;
	}

	bool ContentProvider::isHttpUrl(const std::string& s)
	{
		if (s.find("http://", 0, 7) == 0)
			return true;

		if (s.find("https://", 0, 8) == 0)
			return true;

		return false;
	}

	std::string ContentProvider::assetFolder() const
	{
		return assetFolderPath;
	}

	bool ContentProvider::isRequestQueueEmpty()
	{
		boost::mutex::scoped_lock lock(requestSync);
		return requestQueue.empty();
	}
}

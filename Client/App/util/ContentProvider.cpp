#include "util/ContentProvider.h"

namespace RBX
{
	ContentId::ContentId()
		: id(), 
		  mimeTypePtr(&Name::getNullName())
	{
	}

	ContentId::ContentId(const char* id)
		: id(id),
		mimeTypePtr(&Name::getNullName())
	{
	}

	ContentId::ContentId(std::string id)
		: id(id),
		mimeTypePtr(&Name::getNullName())
	{
	}

	bool ContentId::isAsset() const
	{
		return id.substr(0, 11) == "rbxasset://";
	}

	bool ContentId::isFile() const
	{
		return id.substr(0, 7) == "file://";
	}

	bool ContentId::isHttp() const
	{
		return id.substr(0, 4) == "http";
	}
}
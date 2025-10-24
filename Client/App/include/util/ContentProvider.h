#pragma once

#include "util/Name.h"

namespace RBX
{
	class ContentId
	{
	private:
		std::string id;
		const Name* mimeTypePtr;
	public:
		ContentId(const ContentId&);
		ContentId();
		ContentId(std::string);
		ContentId(const char*);
		ContentId(std::string, const Name&);
		const Name& mimeType() const;
		const char* c_str() const;
		const std::string& toString() const;
		bool isNull() const;
		bool isFile() const;
		bool isAsset() const;
		bool isHttp() const;
		~ContentId() 
		{
		};
		ContentId& operator=(const ContentId&);
	public:
		static ContentId fromUrl(const std::string&);
		static ContentId fromFile(const std::string&);
		static ContentId fromAssets(const std::string&);
		static ContentId fromMD5Hash(const std::string&);
	};
}
